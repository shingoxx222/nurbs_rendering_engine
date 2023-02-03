#include "CRayTracer.h"

CRayTracer::CRayTracer() : mDetectorPtr(0), mInitOK(false), mPhantomPtr(0), mSourcePtr(0)
{
}

void CRayTracer::Init()
{
    size_t ix, iy;
    size_t nx(mDetectorPtr->GetNPixelX());
    size_t ny(mDetectorPtr->GetNPixelY());
    CIntersectionListBase* IntersectionListPtr;
    if (mInitOK) {
        return;
    }
    if (!mDetectorPtr || !mPhantomPtr || !mSourcePtr) {
        throw 1;
    }
    if (!mThicknessMap.Init(ny, nx)) {
        throw 1;
    }
    for (iy = 0; iy < ny; ++iy) {
        for (ix = 0; ix < nx; ++ix) {
            IntersectionListPtr = new CIntersectionListBase;
            if (!IntersectionListPtr) {
                throw 1;
            }
            mThicknessMap[iy][ix] = IntersectionListPtr;
        }
    }
    mInitOK = true;
}
