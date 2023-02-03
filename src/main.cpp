#include <iostream>
using namespace std;
#include "CIntersectionList.h"
#include "CIntersection.h"
#include "CPhantomObject.h"


int main() {
  // test intersection merging
  CIntersectionListBase IntersectionList;
  CIntersectionListBase IntersectionListToMerge;

  float Depth;
  std::size_t ObjectPtr;
  CIntersection Intersection;
  std::size_t i, Counter;

  while (cin.good()) {
    cout << "Enter number of intersection pairs: ";
    cin >> Counter;
    if (Counter == 0) {
      break;
    }
    for (i = 1; i <= Counter; ++i) {
      cout << "  Entry ObjectPtr # " << i << " : ";
      cin >> ObjectPtr;
      cout << "  Entry depth: ";
      cin >> Intersection.Depth;
      Intersection.ObjectPtr = reinterpret_cast<CPhantomObjectBase*>(ObjectPtr);
      IntersectionListToMerge.AddIntersection(Intersection);
      cout << "  Exit depth: ";
      cin >> Intersection.Depth;
      Intersection.ObjectPtr = 0; // 0 should be mandatory for exit intersection
      IntersectionListToMerge.AddIntersection(Intersection);
    }
    cout << "\nNow merging lists, result:\n\n";
    IntersectionList.AddIntersectionsOfObject(IntersectionListToMerge.GetList());
    // clear merge list
    IntersectionListToMerge.Clear();
    cout << IntersectionList;
  }
}