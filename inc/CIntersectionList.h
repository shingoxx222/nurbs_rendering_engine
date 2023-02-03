#ifndef CINTERSECTIONLIST_H
#define CINTERSECTIONLIST_H

#include <vector>
#include <list>
#include <iostream>
#include "CIntersection.h"
#include "CPhantomObject.h"

class CIntersectionListBase {
public:
    CIntersectionListBase() {}
    void AddIntersectionsOfObject(const std::list<float>& IntersectionsOfObject, CPhantomObjectBase* ObjectPtr);
    void AddIntersection(const CIntersection& Intersection) { mIntersectionList.push_back(Intersection); };
	float CalcDepthSum();
	void Clear();
    const int GetSize() const { return mIntersectionList.size(); }
    const std::list<CIntersection>& GetList() const { return mIntersectionList; }
    void Sort() { mIntersectionList.sort(); }
    void Unique() { mIntersectionList.unique(); }
    // friend std::ostream& operator<< (std::ostream& out, const CIntersectionListBase& IntersectionList);

private:
    std::list<CIntersection> mIntersectionList;

};

std::ostream& operator<< (std::ostream& out, const CIntersectionListBase& IntersectionList);

/*
class CIntersectionListNode {
public:
CIntersectionListNode();
CIntersectionListNode(float DepthInit, CPhantomObjectBase* ObjectPtrInit);
float Depth;   // depth of an intersection (= projected on direction vector from source point to pixel)
CPhantomObjectBase* ObjectPtr;
};
*/

#endif
