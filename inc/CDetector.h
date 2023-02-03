#ifndef CDETECTOR_H
#define CDETECTOR_H

#include "CCoordinateSystem.h"

class CDetector : public CCoordinateSystemFloat {
public:
    CDetector();

    std::size_t GetNPixelX() const;
	std::size_t GetNPixelY() const;
    float GetPixelPitchX() const;
    float GetPixelPitchY() const;
	float GetResolution() const;

    void SetNPixelX(std::size_t NPixelX);
    void SetNPixelY(std::size_t NPixelX);
    void SetPixelPitchX(float PixelPitchX);
    void SetPixelPitchY(float PixelPitchY);
	void SetResolution(float Resolution);


private:
	std::size_t mNPixelX;
	std::size_t mNPixelY;
    float mPixelPitchX;
    float mPixelPitchY;
	float mResolution;
};

#endif

