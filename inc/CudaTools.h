#ifndef CUDATOOLS_H
#define CUDATOOLS_H

#include <vector_types.h>
#include "CVector3D.H"

inline float3 make_float3(CVector3DFloat& Source)
{
	float3 t; t.x = Source.x; t.y = Source.y; t.z = Source.z; return t;
}

#endif