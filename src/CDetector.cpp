#include "CDetector.h"

using namespace std;

CDetector::CDetector()
{
	SetNPixelX(3000);
	SetNPixelY(3000);
	SetResolution(0.05f);
}

std::size_t CDetector::GetNPixelX() const
{
    return mNPixelX;
}

std::size_t CDetector::GetNPixelY() const
{
    return mNPixelY;
}

float CDetector::GetPixelPitchX() const
{
    return mPixelPitchX;
}

float CDetector::GetPixelPitchY() const
{
    return mPixelPitchY;
}

float CDetector::GetResolution() const
{
	return mResolution;
}

void CDetector::SetNPixelX(std::size_t NPixelX)
{
    mNPixelX = NPixelX;
}

void CDetector::SetNPixelY(std::size_t NPixelY)
{
    mNPixelY = NPixelY;
}

void CDetector::SetPixelPitchX(float PixelPitchX)
{
    mPixelPitchX = PixelPitchX;
}

void CDetector::SetPixelPitchY(float PixelPitchY)
{
    mPixelPitchY = PixelPitchY;
}

void CDetector::SetResolution(float Resolution)
{
	mResolution = Resolution;
}
