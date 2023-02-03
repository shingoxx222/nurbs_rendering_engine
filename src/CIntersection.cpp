#include "CIntersection.h"

CIntersection::CIntersection() : ObjectPtr(0), Depth(0.0f)
{
}

CIntersection::CIntersection(float DepthInit, CPhantomObjectBase * ObjectPtrInit) :
    Depth(DepthInit), ObjectPtr(ObjectPtrInit)
{
}

bool CIntersection::operator<(const CIntersection & a) const
{
    return Depth < a.Depth;
}

bool CIntersection::operator>(const CIntersection & a) const
{
    return Depth > a.Depth;
}

bool CIntersection::operator<=(const CIntersection & a) const
{
    return Depth <= a.Depth;
}

bool CIntersection::operator>=(const CIntersection & a) const
{
    return Depth >= a.Depth;
}

bool CIntersection::operator==(const CIntersection & a) const
{
    return Depth == a.Depth;
}

bool CIntersection::operator!=(const CIntersection & a) const
{
    return Depth != a.Depth;
}

std::ostream & operator<<(std::ostream & out, const CIntersection & Intersection)
{
    out << Intersection.Depth << " --> " << Intersection.ObjectPtr;
    return out;
}
