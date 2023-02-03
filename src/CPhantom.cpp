#include "CPhantom.h"
#include "CPhantomObject.h"
#include "CError.h"
#include <fstream>
#include "CudaTools.h"

using namespace std;

CPhantomNURBS::CPhantomNURBS()
{
}

CPhantomNURBS::~CPhantomNURBS()
{
    DeleteObjectList();
}

void CPhantomNURBS::CalculateProjection()
{
	//float3 CamPos;
	//CamPos = make_float3(mCoordinateSystemPtr->GetOrigin());
	std::cout<< Nurbs.GetNumObjects() << " NURBS patches to be calculated...\n";

	for (size_t n = 0; n < Nurbs.GetNumObjects(); ++n) {

		

		CPhantomObjectBase* PhantomObject = new CPhantomObjectBase;
		std::vector<std::list<float>> IntersectionList{ (mWidth * mHeight) };

		//std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
		/*sampling and projectiom*/
		//std::cout << "projection\n";
		for (size_t i = 0; i <= mSampleDensity; i++)
		{
			for (size_t j = 0; j <= mSampleDensity; j++)
			{
				float u = static_cast<float>(i) / mSampleDensity;
				float v = static_cast<float>(j) / mSampleDensity;
				float3 surface_point = Nurbs.SurfacePoint(u, v, n);
				//reallocate memory for loading control points of next iteration
				Nurbs.ResetCP();  // deletes control points
				//float3 D = surface_point - CamPos;
				//float3 D = surface_point - mCamPos;
				//normal of the surface point
				//float3 surface_point_u = Nurbs.SurfaceDerU(u, v, n);
				//float3 surface_point_v = Nurbs.SurfaceDerV(u, v, n);
				
				//float3 normal = optix::cross(surface_point_u, surface_point_v);
				//array[i * 100 + j].projection = array[i * 100 + j].projection = CalPlaneLineIntersectPoint(mDetectorNormal, mLookAt, D, mCamPos);
				array[i * mSampleDensity + j].projection = CalPlaneLineIntersectPoint2(surface_point);
				array[i * mSampleDensity + j].depth = distance_sqrt(surface_point, mCamPos);
			}
		}
		//std::chrono::steady_clock::time_point end1 = std::chrono::steady_clock::now();
		//std::cout << "projection time = " << std::chrono::duration_cast<std::chrono::microseconds>(end1 - begin).count() / 1000.0 << "ms" << std::endl;
		/*Rendering Interpolation*/
		//std::cout << "rendering\n";
		for (int i = 0; i <= mSampleDensity; i++)//u
		{
			for (int j = 0; j <= mSampleDensity; j++)//v
			{
				//calculation of index of triangles; neighbors1(2) contain the three corners of a triangle each represented by the index of a sampled surface point
				int neighbours1[3] = { i * mSampleDensity + j, (i + 1) * mSampleDensity + j , i * mSampleDensity + j + 1 };
				int neighbours2[3] = { (i + 1) * mSampleDensity + j + 1, (i + 1) * mSampleDensity + j , i * mSampleDensity + j + 1 };

				check_pixel(array, neighbours1, IntersectionList); // check triangles for containing detector pixel centers and if so add to intersection list of found detector pixels 
				check_pixel(array, neighbours2, IntersectionList);
				//cout << "neighbours1[0]: " << array_in[neighbours1[0]] << "neighbours1[1]: " << array_in[neighbours1[1]] << "neighbours1[2]: " << array_in[neighbours1[2]] << "\n";
				//cout << "\r" << (100 * i + j)/100 << "%";
				//cout.flush();
			}
		}
		//cout << pixel.size();
		//std::chrono::steady_clock::time_point end2 = std::chrono::steady_clock::now();
		//std::cout << "rendering time = " << std::chrono::duration_cast<std::chrono::microseconds>(end2 - end1).count() / 1000.0 << "ms" << std::endl;

		//sort intersection list for each pixel
		for (int i = 0; i < mWidth; i++)
		{
			for (int j = 0; j < mHeight; j++)
			{
				//presort the list
				IntersectionList[i * mHeight + j].sort();
				IntersectionList[i * mHeight + j].unique();

				if (IntersectionList[i * mHeight + j].size() > 1 && IntersectionList[i * mHeight + j].size() % 2 == 0) {
					pixel[i * mHeight + j].AddIntersectionsOfObject(IntersectionList[i * mHeight + j], PhantomObject);
				}
				result[i * mHeight + j] = pixel[i * mHeight + j].CalcDepthSum();
			}
		}
		//std::chrono::steady_clock::time_point end3 = std::chrono::steady_clock::now();
		//std::cout << "sorting time = " << std::chrono::duration_cast<std::chrono::microseconds>(end3 - end2).count() / 1000.0 << "ms" << std::endl;

	}
}

inline int CPhantomNURBS::min(const int a, const int b)
{
	return a < b ? a : b;
}

inline int CPhantomNURBS::max(const int a, const int b)
{
	return a > b ? a : b;
}

inline float CPhantomNURBS::distance(float3 a, float3 b)
{
	return (abs(a.x - b.x) + abs(a.z - b.z));
}

inline float CPhantomNURBS::distance_sqrt(float3 a, float3 b)
{
	return sqrt(pow((a.x - b.x), 2) + pow((a.y - b.y), 2) + pow((a.z - b.z), 2));
}

//calculation of the projected point on detecor
//input: vector of plane, point of plane, vector of line, point of line
//output: 3D coordinate of prohected points on detecor
float3 CPhantomNURBS::CalPlaneLineIntersectPoint(float3 planeVector, float3 planePoint, float3 lineVector, float3 linePoint)
{
	float vpt = lineVector.x * planeVector.x + lineVector.y * planeVector.y + lineVector.z * planeVector.z;
	if (vpt >= -1e-5f && vpt <= 1e-5f) return(make_float3(NULL, NULL, NULL));
	else
	{
		float t = ((planePoint.x - linePoint.x) * planeVector.x + (planePoint.y - linePoint.y) * planeVector.y + (planePoint.z - linePoint.z) * planeVector.z) / vpt;
		return (make_float3(linePoint.x + lineVector.x*t, linePoint.y + lineVector.y*t, linePoint.z + lineVector.z*t));
	}
}

float3 CPhantomNURBS::CalPlaneLineIntersectPoint2(float3 PointToBeProjected)
{
	CVector3DFloat p(mCoordinateSystemPtr->ToDerCoords(CVector3DFloat(PointToBeProjected.x, PointToBeProjected.y, PointToBeProjected.z)));
	CVector3DFloat CamPos(mCoordinateSystemPtr->ToDerCoords(CVector3DFloat(mCamPos.x, mCamPos.y, mCamPos.z)));
	float Scale(CamPos.y / (CamPos.y - p.y));
	CVector3DFloat v(mCoordinateSystemPtr->ToRefCoords(CamPos + Scale*(p - CamPos)));
	return make_float3(v.x, v.y, v.z);
}

//render pixels if they are inside of triangles
//input: projected points with depth, triangle index
//output: list of intersections for each pixel 
int CPhantomNURBS::check_pixel(pixel_payload * data, int * neighbour, std::vector<std::list<float>>& pixel)
{
	// border...: defines corners of detector in real world CS
	CVector3DFloat DetectorZeroPixelOffset(-0.5*mWidth * mResolution, 0.0f, -0.5*mHeight * mResolution);
	CVector3DFloat PixelCoords;

	int3 close_pixel1, close_pixel2, close_pixel3;
	//check if the triangle is valid
	if (data[neighbour[0]].depth > 1.e20f || data[neighbour[1]].depth > 1.e20f || data[neighbour[2]].depth > 1.e20f)
		return -1;
	else if (distance(data[neighbour[0]].projection, data[neighbour[1]].projection) > 100 || distance(data[neighbour[0]].projection, data[neighbour[2]].projection) > 100 || distance(data[neighbour[1]].projection, data[neighbour[2]].projection) > 100)
		return -1;
	else 
	{
		//find nearest pixel coordinate
		PixelCoords = (mCoordinateSystemPtr->ToDerCoords(
			CVector3DFloat(data[neighbour[0]].projection.x, data[neighbour[0]].projection.y, data[neighbour[0]].projection.z))
			- DetectorZeroPixelOffset) / mResolution;
		close_pixel1.x = round(PixelCoords.x);
		// close_pixel1.y = round(PixelCoords.y);
		close_pixel1.z = round(PixelCoords.z);
		PixelCoords = (mCoordinateSystemPtr->ToDerCoords(
			CVector3DFloat(data[neighbour[1]].projection.x, data[neighbour[1]].projection.y, data[neighbour[1]].projection.z))
			- DetectorZeroPixelOffset) / mResolution;
		close_pixel2.x = round(PixelCoords.x);
		// close_pixel2.y = round(PixelCoords.y);
		close_pixel2.z = round(PixelCoords.z);
		PixelCoords = (mCoordinateSystemPtr->ToDerCoords(
			CVector3DFloat(data[neighbour[2]].projection.x, data[neighbour[2]].projection.y, data[neighbour[2]].projection.z))
			- DetectorZeroPixelOffset) / mResolution;
		close_pixel3.x = round(PixelCoords.x);
		// close_pixel3.y = round(PixelCoords.y);
		close_pixel3.z = round(PixelCoords.z);
		//check if triangle is in the scope of detector
		if (((close_pixel1.x < 0) || (close_pixel1.x >= mWidth) || (close_pixel1.z < 0) || (close_pixel1.z >= mHeight)) &&
			((close_pixel2.x < 0) || (close_pixel2.x >= mWidth) || (close_pixel2.z < 0) || (close_pixel2.z >= mHeight)) &&
			((close_pixel3.x < 0) || (close_pixel3.x >= mWidth) || (close_pixel3.z < 0) || (close_pixel3.z >= mHeight)) ) {
			return 1;
		}
		int minx = min(close_pixel1.x, min(close_pixel2.x, close_pixel3.x)) - 10;
		int maxx = max(close_pixel1.x, max(close_pixel2.x, close_pixel3.x)) + 10;
		int minz = min(close_pixel1.z, min(close_pixel2.z, close_pixel3.z)) - 10;
		int maxz = max(close_pixel1.z, max(close_pixel2.z, close_pixel3.z)) + 10;
		minx = minx < 0 ? 0 : minx;
		minz = minz < 0 ? 0 : minz;
		maxx = maxx >(mWidth - 1) ? (mWidth - 1) : maxx;
		maxz = maxz >(mHeight - 1) ? (mHeight - 1) : maxz;
		//loop over rectangular area to search pixels
		for (int x = minx; x <= maxx; ++x)
		{
			for (int y = minz; y <= maxz; ++y)
			{
				float3 p = make_float3(mCoordinateSystemPtr->ToRefCoords(CVector3DFloat(x*mResolution - 0.5*mWidth * mResolution, 0.0f, y*mResolution - 0.5*mHeight * mResolution)));
				//float3 p = make_float3(x*mResolution - 0.5*mWidth * mResolution + mLookAt.x, 0.0f, y*mResolution - 0.5*mHeight * mResolution + mLookAt.z);
				if (inTrig(p, data[neighbour[0]], data[neighbour[1]], data[neighbour[2]]))
				{
					float Intersection;
					// linearly interpolate depth between triangle points at detector center
					Intersection = interpolation(p, data[neighbour[0]], data[neighbour[1]], data[neighbour[2]]);
					pixel[x * mHeight + y].push_back(Intersection);

				}
			}
		}
	}
	return (1);
}
//linear interpolation of three points in triangle
float CPhantomNURBS::interpolation(float3 p, pixel_payload p1, pixel_payload p2, pixel_payload p3)
{
	float w1 = ((p2.projection.z - p3.projection.z)*(p.x - p3.projection.x) + (p3.projection.x - p2.projection.x)*(p.z - p3.projection.z)) / ((p2.projection.z - p3.projection.z)*(p1.projection.x - p3.projection.x) + (p3.projection.x - p2.projection.x)*(p1.projection.z - p3.projection.z));
	float w2 = ((p3.projection.z - p1.projection.z)*(p.x - p3.projection.x) + (p1.projection.x - p3.projection.x)*(p.z - p3.projection.z)) / ((p2.projection.z - p3.projection.z)*(p1.projection.x - p3.projection.x) + (p3.projection.x - p2.projection.x)*(p1.projection.z - p3.projection.z));
	float w3 = 1 - w1 - w2;

	float d = w1*p1.depth + w2*p2.depth + w3*p3.depth;
	return d;
}
//check if a point is inside of a triangle
bool CPhantomNURBS::inTrig(float3 p, pixel_payload p1, pixel_payload p2, pixel_payload p3)
{

	float signOfTrig = (p2.projection.x - p1.projection.x)*(p3.projection.z - p1.projection.z) - (p2.projection.z - p1.projection.z)*(p3.projection.x - p1.projection.x);
	float signOfAB = (p2.projection.x - p1.projection.x)*(p.z - p1.projection.z) - (p2.projection.z - p1.projection.z)*(p.x - p1.projection.x);
	float signOfCA = (p1.projection.x - p3.projection.x)*(p.z - p3.projection.z) - (p1.projection.z - p3.projection.z)*(p.x - p3.projection.x);
	float signOfBC = (p3.projection.x - p2.projection.x)*(p.z - p2.projection.z) - (p3.projection.z - p2.projection.z)*(p.x - p2.projection.x);

	return ((signOfAB * signOfTrig > 0) && (signOfCA * signOfTrig > 0) && (signOfBC * signOfTrig > 0));
}

void CPhantomNURBS::SetPixelMap(CCoordinateSystemFloat * CoordinateSystemPtr, CArray2DFloat * PixelMapPtr)
{
	mCoordinateSystemPtr = CoordinateSystemPtr;
	mPixelMapPtr = PixelMapPtr;
	result = mPixelMapPtr->GetPtr();
}

void CPhantomNURBS::DeleteObjectList()
{
    CPhantomObjectBase* ptr;
    while (!mObjectList.empty()) {
        ptr = mObjectList.back();
        if (ptr) {
            delete ptr;
        }
        mObjectList.pop_back();
    }
}



CPhantomBase::CPhantomBase()
{
}
