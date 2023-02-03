#ifndef CPHANTOM_H
#define CPHANTOM_H

#include "CPhantomObject.h"
#include "CProjection.h"
#include "CArray2D.h"
#include "CNurbs.h"
#include "CCoordinateSystem.h"
#include "CIntersectionList.h"
#include "CDetector.h"
#include "CVector3D.H"
#include <chrono>
#include <vector>

struct pixel_payload {
	float3 projection;
	float depth;
};

inline float3 make_float3(float x, float y, float z)
{
	float3 t; t.x = x; t.y = y; t.z = z; return t;
}

class CPhantomBase : public CCoordinateSystemFloat {
public:
    CPhantomBase();

	virtual void CalculateProjection() {}
	virtual bool Read(std::string FileName) { return true; }
    void SetProjectionSet(const CArray2DBase<CProjection, size_t>& ProjectionSet);
    const CArray2DBase<CIntersectionListBase*, size_t>& GetIntersecionMap() const;

private:
    CArray2DBase<CProjection, size_t> mProjectionSet;
    CArray2DBase<CIntersectionListBase*, size_t> mIntersectionMap;
};

class CPhantomNURBS : public CPhantomBase {
public:
    CPhantomNURBS();
    ~CPhantomNURBS();

	virtual bool Read(std::string FileName) { return Nurbs.ParseInputFile(FileName); }

	virtual void CalculateProjection();

	inline int min(const int a, const int b);

	inline int max(const int a, const int b);

	inline float distance(float3 a, float3 b);

	inline float distance_sqrt(float3 a, float3 b);

	float3 CalPlaneLineIntersectPoint(float3 planeVector, float3 planePoint, float3 lineVector, float3 linePoint);
	float3 CalPlaneLineIntersectPoint2(float3 PointToBeProjected);

	int check_pixel(pixel_payload* data, int* neighbour, std::vector<std::list<float>> &pixel);

	float interpolation(float3 p, pixel_payload p1, pixel_payload p2, pixel_payload p3);

	bool inTrig(float3 p, pixel_payload p1, pixel_payload p2, pixel_payload p3);

	float* GetDepthImage() const { return result; }

	void SetPixelMap(CCoordinateSystemFloat* CoordinateSystemPtr, CArray2DFloat* PixelMapPtr);

	void SetSampleDensity(size_t density) { mSampleDensity = density; }

	void SetCamPos(CVector3DFloat CamPos) {
		mCamPos = make_float3(CamPos.x, CamPos.y, CamPos.z);
	}


	void SetUpScene(CDetector Detector) {
		mWidth = Detector.GetNPixelX();
		mHeight = Detector.GetNPixelY();
		mResolution = Detector.GetResolution();
		//mCamPos = make_float3(80.0f, 1000.0f, -50.0f);
		//mCoordinateSystemPtr->Translate(CVector3DFloat(80.0f, 0.0f, -50.0f));
		// mLookAt = make_float3(80.0f, 0.0f, -50.0f);
		//mDetectorNormal = make_float3(0.0f, 1.0f, 0.0f);
		pixel.assign(mWidth * mHeight, CIntersectionListBase());
	}

    void DeleteObjectList();

private:
	/*Camera setting phantom*/
	float3 mCamPos;
	// float3 mLookAt;
	//float3 mDetectorNormal;
	float  mResolution;
	size_t mWidth;
	size_t mHeight;

	size_t mSampleDensity;
	CCoordinateSystemFloat* mCoordinateSystemPtr;

	

    std::vector<CPhantomObjectBase*> mObjectList;
	CNurbs Nurbs;
	CArray2DFloat* mPixelMapPtr;
	float* result;
	pixel_payload *array = new pixel_payload[150*150];
	std::vector<CIntersectionListBase> pixel;
};

#endif
