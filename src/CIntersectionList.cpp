#include "CIntersectionList.h"
#include <list>
#include <limits>
using namespace std;

void CIntersectionListBase::AddIntersectionsOfObject(const std::list<float>& IntersectionsOfObject, CPhantomObjectBase* ObjectPtr)
{
    // iterators for the destination list
    list<CIntersection>::iterator it(mIntersectionList.begin());
    list<CIntersection>::iterator itPrevious, itDest;
    list<CIntersection>::iterator itEnd(mIntersectionList.end());

    // iterators for the object (source) list
    list<float>::const_iterator itObj, itObjPrevious;
    const list<float>::const_iterator itObjEnd(IntersectionsOfObject.end());

    CPhantomObjectBase* PreviousObjectPtr(0);

    size_t Counter;

    // assume that sorting and consistency check occurs before, i.e. IntersectionOfObjects should have a guaranteed correct syntax
    // IntersectionsOfObject.sort();
	if (IntersectionsOfObject.size() % 2 != 0) {
		cout << "Error, uneven number of intersections\n";
		cin >> Counter;
	}
    for (itObj = IntersectionsOfObject.begin(); itObj != itObjEnd; ++itObj)
    {
        // next object intersection enters object
        // go through the destination list to ignore all intersection points with lower depth,
        // they should not be changed

        if ((it != itEnd) && (it->Depth < *itObj)) {
            ++it;
            while ((it != itEnd) && (it->Depth < *itObj))
            {
                // object in destination list has smaller depth, go to next intersection in destination list
                ++it;
            }
            // remember the object pointer of the last intersection with lower depth, it might be needed later
            // the if / while construct ensures that ObjectPtr (by default set to 0) is only updated if a previous intersection exists
            itPrevious = it;
            --itPrevious;
            PreviousObjectPtr = itPrevious->ObjectPtr;
            ++itPrevious;  // finally, in itPrevious remember first destination intersection having a larger depth 
        }
        else {
            // remember the object pointer of the last intersection with lower depth, it might be needed later
            itPrevious = it;
        }

        // itObj now points to an intersection with depth between that of *(it - 1) and *(it)
        // distinguish between three cases:
        //  1. source object does not overlap any destination intersection
        //     --> source object is contained in destination object, two intersections to be inserted
        //  2. source object overlaps one destination intersection  --> one intersection to be inserted
        //  3. source object overlaps more than one destination intersection 

        itObjPrevious = itObj;
        ++itObj;  // itObj now contains the exit intersection

                  // count how many destination intersections are overlapped
        Counter = 0;
        while ((it != itEnd) && (it->Depth <= *itObj)) {
            ++it;
            ++Counter;
        }

        switch (Counter)
        {
        case 0:
            // case 1: source object does not overlap any destination intersection
            // insert two new intersection in destination list.
			mIntersectionList.insert(it, CIntersection(*itObjPrevious, ObjectPtr));
			mIntersectionList.insert(it, CIntersection(*itObj, PreviousObjectPtr));

            // mIntersectionList.insert(it, itObjPrevious, itObj);  // inserts copies from itObjPrevious to the one prior of itObj (--> entry/exit intersections)
                                                                 // the second inserted intersection needs to get an updated object pointer
            // itDest = it;
            // --itDest;
            // itDest->ObjectPtr = PreviousObjectPtr;
            continue;
        case 1:
            // case 2: source object overlaps one destination intersection  --> one intersection to be inserted
            mIntersectionList.insert(itPrevious, CIntersection(*itObjPrevious, ObjectPtr));
            // the overlapped destination intersection just needs to get an updated depth defined by the source object exit intersection
            itPrevious->Depth = *itObj;
            continue;
        default:
            // case 3: source object overlaps more than one destination intersection
            // no new intersections to be inserted in list, potentially even intersections to be deleted
            // information of itPrevious will not be needed;
            // overwrite those following itPrevious with the destination object entry intersection
            itPrevious->Depth = *itObjPrevious;
			itPrevious->ObjectPtr = ObjectPtr;
			++itPrevious;
            itDest = it;
            --itDest;
            mIntersectionList.erase(itPrevious, itDest);
            // itDest needs to get an updated Depth
            itDest->Depth = *itObj;
            continue;
        }
	}
}

float CIntersectionListBase::CalcDepthSum()
{
	float DepthIn, DepthSum(0.0f);
	list<CIntersection>::const_iterator it(mIntersectionList.begin()), itEnd(mIntersectionList.end());
	while (it != itEnd) {
		if (it->ObjectPtr) {
			DepthIn = it->Depth;
			++it;
			DepthSum += (it->Depth - DepthIn);
		}
		else {
			++it;
		}
	}
	return DepthSum;
}

void CIntersectionListBase::Clear()
{
    mIntersectionList.erase(mIntersectionList.begin(), mIntersectionList.end());
}

/*
CIntersectionListNode::CIntersectionListNode() :
Depth(-numeric_limits<float>::max()),
ObjectPtr(0)
{
}

CIntersectionListNode::CIntersectionListNode(float DepthInit, CPhantomObjectBase* ObjectPtrInit) :
Depth(DepthInit),
ObjectPtr(ObjectPtrInit)
{
}
*/

std::ostream & operator<<(std::ostream & out, const CIntersectionListBase & IntersectionList)
{
    list<CIntersection>::const_iterator it(IntersectionList.GetList().begin()), itEnd(IntersectionList.GetList().end());
    while (it != itEnd) {
        out << *it << std::endl;
        ++it;
    }
    return out;
}
