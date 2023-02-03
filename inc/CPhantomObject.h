#ifndef CPHANTOMOBJECT_H
#define CPHANTOMOBJECT_H

#include <fstream>
#include "CCoordinateSystem.h"


class CPhantomObjectBase : public CCoordinateSystemFloat {
public:
    CPhantomObjectBase() {}
};

class CPhantomObjectNURBS : public CPhantomObjectBase {
public:
    CPhantomObjectNURBS() {}

    // implement here whatever is needed for NURBS objects
};

#endif
