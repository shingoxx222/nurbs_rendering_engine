#include <optix.h>
#include <vector_types.h>
#include <stdio.h>
#include <string.h>
#include <optixu/optixpp_namespace.h>
#include <optixu/optixu_math_namespace.h>
#include <optixu/optixu_math_stream_namespace.h>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <fstream>
#include <sstream>
#include <math.h>
#include <stdint.h>
#include <fstream>
#include "CNurbs.h"
#include "CError.h"
#include "CIntersection.h"
#include "CIntersectionList.h"
#include "CPhantom.h"
#include "CDetector.h"
#include "CCoordinateSystem.h"
#include "CArray2D.h"

/*Camera setting heart
float3 cam_pos = make_float3(80.0f, -1000.0f, -50.0f);
float3 look_at = make_float3(80.0f, 0.0f, -50.0f);
float3 detector_normal = make_float3(0.0f, 1.0f, 0.0f);*/
const std::size_t DetectorNPixelX(256);
const std::size_t DetectorNPixelY(256);
const float pi(atan(1.0f)*4.0);
const float deg2rad(pi / 180.f);

int main()
{
	CDetector Detector;
	CPhantomNURBS Objects;
	CCoordinateSystemFloat DetectorCS;
	std::string FileName = "male_pt146.obj";
	//std::string FileName = "male_infant.obj";
	//std::string FileName = "dias_pericardium.obj";
	CArray2DFloat DepthMap;  // contains the final image, unit: water thickness in mm

	// coordinate system: detector in xz-plane, source in positive y-direction; unit: mm
	CVector3DFloat CamPos(80.0f, 3000.0f, -50.0f);    // X-ray source position

	Detector.SetNPixelX(DetectorNPixelX);
	Detector.SetNPixelY(DetectorNPixelY);
	Detector.SetResolution(400.0f / DetectorNPixelX);  // pixel pitch
	//Detector.SetResolution(0.2f);

	DetectorCS.Translate(CVector3DFloat(80.0f, 0.0f, -50.0f));
	Objects.SetCamPos(CamPos);

	//DetectorCS.RotateYRef(90.0*deg2rad);

	//std::string FileName = "male_infant.obj";
	DepthMap.Init(DetectorNPixelY, DetectorNPixelX);
	Objects.SetPixelMap(&DetectorCS, &DepthMap);
	Objects.Read(FileName);  // reads in NURBS data file, note: label reading still to be implemented
	Objects.SetUpScene(Detector);
	Objects.SetSampleDensity(60); // sets the number of sample intervals in u-v- space
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	Objects.CalculateProjection();
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	std::cout << "Total execution time = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() / 1000.0 << "ms" << std::endl;
    
	FILE * file = fopen("depth.bin", "wb");
	fwrite(Objects.GetDepthImage(), sizeof(*Objects.GetDepthImage()), DetectorNPixelX * DetectorNPixelY, file);

	std::cout << "Sucess!\n";

	getchar();
	return 0;
}