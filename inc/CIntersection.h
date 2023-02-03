#ifndef CINTERSECTION_H
#define CINTERSECTION_H

#include "CPhantomObject.h"

class CIntersection {
public:
    CIntersection();
    CIntersection(float DepthInit, CPhantomObjectBase* ObjectPtrInit);

    bool operator<(const CIntersection& a) const;
    bool operator>(const CIntersection& a) const;
    bool operator<=(const CIntersection& a) const;
    bool operator>=(const CIntersection& a) const;
    bool operator==(const CIntersection& a) const;
    bool operator!=(const CIntersection& a) const;

    float Depth;
    CPhantomObjectBase* ObjectPtr;
};

std::ostream& operator<< (std::ostream &out, const CIntersection &Intersection);

#endif
