#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <locale> 
#include <algorithm>
#include "vector_types.h"
#include <optixu/optixu_math_namespace.h>
using namespace std;

#include "CNurbs.h"
#include "CError.h"

bool CNurbs::ParseInputFile(string& FileName) {
    ifstream File;   // this is the C++ equivalent to the FILE type in C (works different)
    CError Error;    // helper tool to print errors
    string s;
    std::locale loc;
    float3 f3;
    unsigned int i;
    float f;
    vector<float> farray;
    vector<float3> f3array;
    vector<unsigned int> iarray;

    File.open(FileName.c_str());   // opens file for reading
    if (!File.good()) {    // method checks if any error occured (then .good() = false)
        Error.FileI(FileName);
        File.close();
        return false;
    }
    cout << "Scanning file " << FileName << "\n";   // C++ equivalent to printf
    while (File.good()) {
        s = "";
        File >> s;   // read in string in file into s
        if (s == "end") {
            // no more content found
            cout << "Finished " << FileName << "\n";   // C++ equivalent to printf
            return File.eof();  // return true if regular end of file was reached; return false if file error occured
        }
		// for now skip label information, indicated by lowercase letter
        if ((s.c_str()[0] == '/') || (s.c_str()[0] == '#') || ((s.c_str()[0] >= 'a') && (s.c_str()[0] <= 'z'))) { // '/' or '#' are comment marker; 
                                                                                                                  // comment found; skip line
            getline(File, s);
            continue;
        }

        if (s == "Control") {
            // Control point was found in file; scan for the value
            if (!File.good()) break;
            while (File.good()) {// in case of a parsing (syntax) error, the "good" flag will be set to false
                getline(File, s);
                File >> f3.x >> f3.y >> f3.z;
                f3array.push_back(f3);
            }
            mVertex.push_back(f3array);
            f3array.clear();
            File.clear();
            continue;
        }

        if (s == "U") {
            // the keyword "U" for the u-knot vector was found in file; scan for the value
            if (!File.good()) break;
            while (File.good()) {
                getline(File, s);
                File >> f;
                farray.push_back(f);
            }
            mKnotU.push_back(farray);
            farray.clear();
            File.clear();
            continue;
        }

        if (s == "V") {
			// the keyword "V" for the v-knot vector was found in file; scan for the value
			if (!File.good()) break;
            while (File.good()) {
                getline(File, s);
                File >> f;
                farray.push_back(f);
            }
            mKnotV.push_back(farray);
            farray.clear();
            File.clear();
            continue;
        }

        if (s.c_str()[0] <= '9' && s.c_str()[0] >= '0') {
			// a number should be followed by a :N or :M syntax indicating the number of control points, i.e. M for U and N for V
            i = atoi(s.c_str());
            File >> s;
            if (s == ":M")
                mNumberM.push_back(i);
            if (s == ":N")
                mNumberN.push_back(i);
            File.clear();
            continue;
        }

        Error.FileConv(FileName);
        Error.AddComment("Don't know keyword \"");
        Error.AddComment(s);
        Error.AddComment("\"\n");
        File.close();
        return false;
    }
    File.close();
    if (File.bad()) {
        // problem to read already opened file
        Error.FileR(FileName);
        return false;
    }
    if (File.fail()) {
        // fail() indicates a syntax error in parsing, or end of file
        if (File.eof()) {
            Error.AddComment("Unexpected end of file found\n");
        }
        // error must have been occured during keyword value reading
        Error.FileConv(FileName);
        Error.AddComment("Syntax error or missing value behind keyword \"");
        Error.AddComment(s);
        Error.AddComment("\"\n");
        return false;
    }
    return File.eof(); // regularly, end of file should be reached (return value true)
}

size_t CNurbs::GetNumPointsN(size_t index_object)
{
	return mNumberM[index_object];
}

size_t CNurbs::GetNumPointsM(size_t index_object)
{
	return mNumberN[index_object];
}

float * CNurbs::GetKnotVectorU(size_t index_object)
{
	float *knot_u = new float[mKnotV[index_object].size()];
	for (int i = 0; i < mKnotV[index_object].size(); ++i) {
		knot_u[i] = mKnotV[index_object][i];
	}

	return knot_u;
}

float * CNurbs::GetKnotVectorV(size_t index_object)
{
	float *knot_v = new float[mKnotU[index_object].size()];
	for (int i = 0; i < mKnotU[index_object].size(); ++i) {
		knot_v[i] = mKnotU[index_object][i];
	}

	return knot_v;
}

float3 * CNurbs::GetVertex(size_t index_object)
{
	float3 *control_points = new float3[mVertex[index_object].size()];
	for (int i = 0; i < mVertex[index_object].size(); ++i) {
		control_points[i] = mVertex[index_object][i];
	}
	
	return control_points;
}
//find the knot span of u
//input: number of control points, degree, u, knot vector
//output: index of the left bracket of knot span
unsigned int CNurbs::FindSpan(unsigned int number_points, unsigned int degree, float u, float * knot)
{
	/*special case*/
	if (u == knot[number_points + 1]) return(number_points);
	/*binary search*/
	int low = degree;
	int high = number_points + 1;
	int mid = (low + high) / 2;
	int mid_prev = 0;
	while (u < knot[mid] || u >= knot[mid + 1])
	{
		if (u < knot[mid]) high = mid;
		else                low = mid;
		mid = (low + high) / 2;
		if (mid == mid_prev) break;
		mid_prev = mid;
	}
	//cout << "span " << mid << "\n";
	return(mid);
}
//calculation of the basis function of u, sum of elements in basis function = 1.0f
//input: found knot span, u, degree, knot vector
//output: basis function in an array, stored in BF
void CNurbs::BasisFuns(unsigned int span, float u, unsigned int degree, float * knot, float * BF)
{
	BF[0] = 1;
	float left[10], right[10];
	float saved, temp;
	for (int j = 1; j <= degree; j++)
	{
		left[j] = u - knot[span + 1 - j];
		right[j] = knot[span + j] - u;
		saved = 0.0f;
		for (int r = 0; r < j; r++)
		{
			if ((right[r + 1] + left[j - r]) == 0) temp = 0;
			else temp = BF[r] / (right[r + 1] + left[j - r]);
			BF[r] = saved + right[r + 1] * temp;
			saved = left[j - r] * temp;
		}
		BF[j] = saved;
	}
}
//calculation of the derivative basis function of u, sum of elements in basis function = 0.0f
//input: found knot span, u, degree, knot vector
//output: derivative basis function in an array
void CNurbs::DerBasisFuns(unsigned int span, float u, unsigned int degree, float * knot, float * DF)
{
	float BF[10];
	BasisFuns(span, u, degree - 1, knot, BF);
	float left, right;
	for (int i = span - degree; i <= span; i++)
	{
		if (knot[i + degree] - knot[i] == 0.0f) left = 0.0f;
		else left = 1 / (knot[i + degree] - knot[i]);
		if (knot[i + degree + 1] - knot[i + 1] == 0.0f) right = 0.0f;
		else right = 1 / (knot[i + degree + 1] - knot[i + 1]);

		if (i == span - degree) DF[i - span + degree] = degree*(0.0f - BF[0] * right);
		else if (i == span) DF[i - span + degree] = degree*(BF[degree - 1] * left);
		else DF[i - span + degree] = degree*(BF[i - span + degree - 1] * left - BF[i - span + degree] * right);

	}
}
//calculation of NURBS surface point
//input: u, v, object index
//output: 3D coordinate of the surface point
float3 CNurbs::SurfacePoint(float u, float v, size_t index_object)
{
	if (u >= 1.0f) u = 0.999f; if (u <= 0.0f) u = 0.001f;
	if (v >= 1.0f) v = 0.999f; if (v <= 0.0f) v = 0.001f;
	float Nu[10];// basis function
	float Nv[10];
	//retrive NURBS parameters
	number_points_n = GetNumPointsN(index_object);
	number_points_m = GetNumPointsM(index_object);
	knot_u = GetKnotVectorU(index_object);
	knot_v = GetKnotVectorV(index_object);
	control_points = GetVertex(index_object);

	unsigned int uspan = FindSpan(number_points_n, degree_p, u, knot_u); // finds left-bracket of the u/v interval 
	BasisFuns(uspan, u, degree_p, knot_u, Nu);  // prepare the basis functions for corresponding interval, stored in Nu
	unsigned int vspan = FindSpan(number_points_m, degree_q, v, knot_v);
	BasisFuns(vspan, v, degree_q, knot_v, Nv);
	unsigned int  uind = uspan - degree_p;  // first index of a base function to be used for calculation of the surface point
	float3 surface_point = optix::make_float3(0);
	for (int i = 0; i <= degree_q; i++)
	{
		unsigned int vind = vspan - degree_q + i;
		for (int j = 0; j <= degree_p; j++)
		{
			surface_point = surface_point + Nv[i] * Nu[j] * control_points[(uind + j) * number_points_m + vind];
		}
	}
	return (surface_point);
}
//calculation of derivative of NURBS surface point on u
//input: u, v, object index
//output: 3D coordinate
float3 CNurbs::SurfaceDerU(float u, float v, size_t index_object)
{
	if (u >= 1.0f) u = 0.999f; if (u <= 0.0f) u = 0.001f;
	if (v >= 1.0f) v = 0.999f; if (v <= 0.0f) v = 0.001f;
	float Nu[10];
	float Nv[10];

	unsigned int uspan = FindSpan(number_points_n, degree_p, u, knot_u);
	DerBasisFuns(uspan, u, degree_p, knot_u, Nu);
	//rtPrintf("u: %f, Nu[0]: %f, Nu[1]: %f,Nu[2]: %f\n", u, Nu[0], Nu[1], Nu[2]);
	unsigned int vspan = FindSpan(number_points_m, degree_q, v, knot_v);
	BasisFuns(vspan, v, degree_q, knot_v, Nv);
	unsigned int uind = uspan - degree_p;
	float3 surface_der_u = optix::make_float3(0);
	for (int i = 0; i <= degree_q; i++)
	{
		unsigned int vind = vspan - degree_q + i;
		for (int j = 0; j <= degree_p; j++)
		{
			surface_der_u = surface_der_u + Nv[i] * Nu[j] * control_points[(uind + j) * number_points_m + vind];
			//surface_der_u = surface_der_u + Nv[i] * Nu[j] * control_points[(uind + j) + vind*number_points_n];
		}
	}
	return (surface_der_u);
}
//calculation of derivative of NURBS surface point on v
//input: u, v, object index
//output: 3D coordinate
float3 CNurbs::SurfaceDerV(float u, float v, size_t index_object)
{
	if (u >= 1.0f) u = 0.999f; if (u <= 0.0f) u = 0.001f;
	if (v >= 1.0f) v = 0.999f; if (v <= 0.0f) v = 0.001f;
	float Nu[10];
	float Nv[10];

	unsigned int uspan = FindSpan(number_points_n, degree_p, u, knot_u);
	BasisFuns(uspan, u, degree_p, knot_u, Nu);
	unsigned int vspan = FindSpan(number_points_m, degree_q, v, knot_v);
	DerBasisFuns(vspan, v, degree_q, knot_v, Nv);
	unsigned int uind = uspan - degree_p;
	float3 surface_der_v = optix::make_float3(0);
	for (int i = 0; i <= degree_q; i++)
	{
		unsigned int vind = vspan - degree_q + i;
		for (int j = 0; j <= degree_p; j++)
		{
			surface_der_v = surface_der_v + Nv[i] * Nu[j] * control_points[(uind + j) * number_points_m + vind];
		}
	}
	
	return (surface_der_v);
}
