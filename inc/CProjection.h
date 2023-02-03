#ifndef CPROJECTION_H
#define CPROJECTION_H

#include "CVector3D.h"

class CProjection {
public:
    CProjection();

    const CVector3DFloat& GetDirection() const;
    const CVector3DFloat& GetSource() const;
    void SetDirection(const CVector3DFloat& DirectionVector);
    void SetProjection(const CVector3DFloat& SourceVector, const CVector3DFloat& DestinationVector);
    void SetSource(const CVector3DFloat& SourceVector);

private:
    CVector3DFloat mSource;
    CVector3DFloat mDirection;
};

#endif

