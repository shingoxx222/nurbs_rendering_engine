#ifndef CNURBS_H
#define CNURBS_H

#include <vector>
#include "vector_types.h"
#include <optixu/optixu_math_namespace.h>
using namespace std;

class CNurbs {

public:
    CNurbs() {}


    bool ParseInputFile(std::string& FileName);
	size_t GetNumObjects() {
		return mKnotU.size();
	}

	size_t GetNumPointsN(size_t index_object);
	size_t GetNumPointsM(size_t index_object);
	float* GetKnotVectorU(size_t index_object);
	float* GetKnotVectorV(size_t index_object);
	float3* GetVertex(size_t index_object);
	void ResetCP() { delete[] control_points; }

	float3 SurfacePoint(float u, float v, size_t index_object);
	float3 SurfaceDerU(float u, float v, size_t index_object);
	float3 SurfaceDerV(float u, float v, size_t index_object);
	unsigned int FindSpan(unsigned int number_points, unsigned int degree, float u, float* knot);
	void BasisFuns(unsigned int span, float u, unsigned int degree, float* knot, float* BF);
	void DerBasisFuns(unsigned int span, float u, unsigned int degree, float* knot, float*DF);

    private:
    vector<vector<float3>> mVertex;
    vector<vector<float>> mKnotU;
    vector<vector<float>> mKnotV;
    vector<unsigned int> mNumberM;
    vector<unsigned int> mNumberN;

	size_t degree_p = 3;
	size_t degree_q = 3;
	size_t number_points_n;
	size_t number_points_m;
	float* knot_u;
	float* knot_v;
	float3* control_points;


};

#endif