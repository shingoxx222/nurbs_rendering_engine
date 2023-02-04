#ifndef CARRAY2D_H
#define CARRAY2D_H

#include <fstream>
#include <vector>
#include "CScale.h"
#include "CBox.h"
#include "CArray.h"
// 2D array class template
// note: commonly, first index ("row index") is referred to y, the second index ("column index") is referred to x


template <class T, class TIndex> class CArray2DBase{
public:
  CArray2DBase() : mArray(0), mNValuesX(0), mNValuesY(0){}
  ~CArray2DBase(){
    DeleteArray();
  }

  T* operator[](TIndex iy){
    return &mArray[iy*mNValuesX];
  }

  CArray2DBase<T, TIndex>& operator=(const CArray2DBase<T, TIndex>& Source) {
    if (this != &Source) {
      if (!Init(Source.GetNValuesY(), Source.GetNValuesX())) {
        // implement exception here
        return *this;
      }
      T* pd(mArray);
      T* pLast(mArray + GetSize());
      T* ps(Source.GetPtr());
      while (pd < pLast) {
        *pd++ = *ps++;
      }
    }
    return *this;
  }

  CArray2DBase<T, TIndex>& operator*=(const T Multiplier) {
    T* pd(mArray);
    T* pLast(mArray + GetSize());
    while (pd < pLast) {
      *pd++ *= Multiplier;
    }
    return *this;
  }

  template <class T2, class TIndex2> CArray2DBase<T, TIndex>& operator=(CArray2DBase<T2, TIndex2>& Source){
    T2 s;
    if (this != reinterpret_cast<CArray2DBase<T, TIndex>*>(&Source)){
      if (!Init(static_cast<TIndex>(Source.GetNValuesY()), static_cast<TIndex>(Source.GetNValuesX()))){
        // implement exception here
        return *this;
      }
      T* pd(mArray);
      T* pLast(mArray + Source.GetSize());
      T2* ps(Source.GetPtr());
      while (pd < pLast){
        s = *ps++;
        *pd++ = static_cast<T>(s);
      }
    }
    return *this;
  }

  void BoxCropToFit(CBox<TIndex>& Box){
    if (Box.GetX2() > mNValuesX){
      Box.SetX2(mNValuesX);
    }
    if (Box.GetY2() > mNValuesY){
      Box.SetY2(mNValuesY);
    }
  }

  void BoxFullField(CBox<TIndex>& Box) const {
    Box.SetBoundary(0, 0, mNValuesX, mNValuesY);
  }

  void DeleteArray(){
    if (mArray){
      delete[] mArray;
      mNValuesX = 0;
      mNValuesY = 0;
      mArray = 0;
    }
  }

  void FillBoxFast(CBox<TIndex>& Box, T Value){
    // keep care, no check for boundary conditions of box!
    T* p, pLastX, pLastY;
    TIndex DeltaX, DeltaY, x1(Box.GetX1()), x2(Box.GetX2()), y1(Box.GetY1()), y2(Box.GetY2());
    DeltaX = x2 - x1;
    DeltaY = mNValuesX - DeltaX;
    p = mArray + y1*mNValuesX + x1;
    pLastY = mArray + y2*mNValuesX;
    while( p < pLastY){
      pLastX = p + DeltaX;
      while (p < pLastX){
        *p++ = Value;
      }
      p += DeltaY;
    }
  }

  T GetAverage() const{
    T* p(mArray);
    T* pLast(mArray + mNValuesX*mNValuesY);
    T Average(0);
    while (p < pLast){
      Average += *p++;
    }
    return Average / (mNValuesX*mNValuesY);
  }

  T GetBoxAverageFast(CBox<TIndex>& Box) const{
    // keep care, no check for boundary conditions of box!
    return GetBoxSumFast(Box) / ((Box.GetX2() - Box.GetX1())*(Box.GetY2() - Box.GetY1()));
  }

  T GetBoxSumFast(CBox<TIndex>& Box) const{
    // keep care, no check for boundary conditions of box!
    T* p;
    T* pLastX;
    T* pLastY;
    TIndex DeltaX, DeltaY, x1(Box.GetX1()), x2(Box.GetX2()), y1(Box.GetY1()), y2(Box.GetY2());
    T Sum(0);
    DeltaX = x2 - x1;
    DeltaY = mNValuesX - DeltaX;
    p = mArray + y1*mNValuesX + x1;
    pLastY = mArray + y2*mNValuesX;
    while (p < pLastY){
      pLastX = p + DeltaX;
      while (p < pLastX){
        Sum += *p++;
      }
      p += DeltaY;
    }
    return Sum;
  }

  T GetMax() const {
    T* p(mArray);
    T* pLast(mArray + mNValuesX*mNValuesY);
    T Max, d;
    if (!mArray) {
      return 0.0;
    }
    Max = *p++;
    while (p < pLast) {
      d= *p++;
      if (d > Max) {
        Max = d;
      }
    }
    return Max;
  }

  T GetMin() const {
    T* p(mArray);
    T* pLast(mArray + mNValuesX*mNValuesY);
    T Min, d;
    if (!mArray) {
      return 0.0;
    }
    Min = *p++;
    while (p < pLast) {
      d = *p++;
      if (d < Min) {
        Min = d;
      }
    }
    return Min;
  }

  TIndex GetNValuesX() const{
    return mNValuesX;
  }

  TIndex GetNValuesY() const{
    return mNValuesY;
  }

  T* GetPtr(){
    return mArray;
  }

  T* GetPtr() const {
    return mArray;
  }

  TIndex GetSize(){
    return mNValuesX*mNValuesY;
  }

  TIndex IndexCropX(TIndex i) {
    if (i >= mNValuesX) {
      return mNValuesX - 1;
    }
    return i;
  }

  TIndex IndexCropY(TIndex i) {
    if (i >= mNValuesY) {
      return mNValuesY - 1;
    }
    return i;
  }

  bool Init(TIndex NValuesY, TIndex NValuesX){
    TIndex NElements(NValuesX*NValuesY);
    if (NElements == mNValuesX * mNValuesY){
      // already initialized, do nothing
      return true;
    }
    DeleteArray();
    if (NElements == 0){
      return true;
    }
    mArray = new T[NElements];
    if (!mArray){
      CError Error;
      Error.OutOfMemory("CArray2D::Init");
      return false;
    }
    mNValuesX = NValuesX;
    mNValuesY = NValuesY;
    return true;
  }

  template <class T2, class TIndex2> void Paste(const CArray2DBase<T2, TIndex2>& Source){
    TIndex DestOffsetX(0), DestOffsetY(0);
    TIndex2 nx(Source.GetNValuesX()), ny(Source.GetNValuesY());
    CBox<TIndex2> SourceBox(0, nx, 0, ny);
    if (nx < mNValuesX){
      // source image size is smaller; calculate an offset such that pasted image is centered
      DestOffsetX = static_cast<TIndex>((mNValuesX - nx) / 2);
    }
    else {
      // source image size is equal or larger: 
      SourceBox.SetX1(static_cast<TIndex2>((nx - mNValuesX) / 2));
      SourceBox.SetSizeX(nx);
    }
    if (ny < mNValuesY){
      // source image size is smaller; calculate an offset such that pasted image is centered
      DestOffsetY = static_cast<TIndex>((mNValuesY - ny) / 2);
    }
    else {
      // source image size is equal or larger: 
      SourceBox.SetY1(static_cast<TIndex2>((ny - mNValuesY) / 2));
      SourceBox.SetSizeY(ny);
    }
    PasteFast(Source, SourceBox);
  }

  template <class T2, class TIndex2> void PasteFast(const CArray2DBase<T2, TIndex2>& Source, CBox<TIndex2>& SourceBox, TIndex DestOffsetX, TIndex DestOffsetY){
    T2* SPtr, SLastXPtr, SLastYPtr;
    T* DPtr;
    TIndex2 Sdx(SourceBox.GetXSize());
    TIndex2 Sdy(Source.GetNValuesX() - Sdx);
    TIndex Ddy(static_cast<TIndex>(mNValuesX - Sdx));
    SPtr = &Source[SourceBox.GetY1()][SourceBox.GetX1()];
    SLastYPtr = &Source[SourceBox.GetY2()][SourceBox.GetX1()];
    DPtr = &mArray[DestOffsetY][DestOffsetX];
    while (SPtr < SLastYPtr){
      SLastXPtr = SPtr + Sdx;
      while (SPtr < SLastXPtr){
        *DPtr++ = *SPtr++;
      }
      SPtr += Sdy;
      DPtr += Sdy;
    }
  }

  template <class T2, class TIndex2> void FillAndPasteFast(CArray2DBase<T2, TIndex2>& Source, T FillValue) {
    // take full source image and automatically center pasted image
    CBox<TIndex2> SourceBox;
    TIndex OffsetX, OffsetY;
    Source.BoxFullField(SourceBox);
    OffsetX = (mNValuesX - Source.GetNValuesX()) / 2;
    OffsetY = (mNValuesY - Source.GetNValuesY()) / 2;
    FillAndPasteFast(Source, SourceBox, OffsetX, OffsetY, FillValue);
  }

  template <class T2, class TIndex2> void FillAndPasteFast(CArray2DBase<T2, TIndex2>& Source, CBox<TIndex2>& SourceBox, TIndex DestOffsetX, TIndex DestOffsetY, T FillValue){
    T2* SPtr;
    T *DPtr, *DPtrStop, *DPtrLast;
    TIndex2 SizeX(SourceBox.GetXSize());
    TIndex2 SizeY(SourceBox.GetYSize());
    TIndex2 SourceToNextLine(Source.GetNValuesX() - SizeX);
    TIndex DestToNextLine(static_cast<TIndex>(mNValuesX - SizeX));
    // order of fill & paste:
    // 1) fill area "below" paste area
    // 2a) fill line until first pixel to be copied
    // 2b) copy a pixel line
    // 2c) repeat 2a & 2b until "middle" area is filled & pasted
    // 3) fill area "above" paste area
    if ((SizeX <= 0) || (SizeY <= 0)){
      // zero size or undefined source ROI
      SetAll(FillValue);
      return;
    }
    DPtr = mArray;
    DPtrStop = mArray + mNValuesX*DestOffsetY + DestOffsetX;   // points to first destination ROI pixel
    DPtrLast = DPtrStop + mNValuesX*(SizeY - 1) + SizeX;  // points to first destination pixel after paste ROI
    SPtr = &Source[SourceBox.GetY1()][SourceBox.GetX1()];  // points to first source ROI pixel
    while (DPtr < DPtrLast){
      while (DPtr < DPtrStop){
        // fill until begin of a paste line
        *DPtr++ = FillValue;
      }
      DPtrStop += SizeX;
      while (DPtr < DPtrStop){
        // copy line
        *DPtr++ = *SPtr++;
      }
      SPtr += SourceToNextLine;  // add offset to source pointer towards next line to be pasted 
      DPtrStop += DestToNextLine; // set stop pointer for the next filling process
    }
    DPtrLast = mArray + mNValuesX*mNValuesY;  // set to first pointer after end of destination array
    while (DPtr < DPtrLast){
      *DPtr++ = FillValue;
    }
  }

  void SetAll(T Value){
    T* p(mArray);
    T* pLast(mArray + mNValuesX*mNValuesY);
    while (p < pLast){
      *p++ = Value;
    }
  }

protected:
  TIndex mNValuesX;
  TIndex mNValuesY;
  T* mArray;
};

typedef CArray2DBase<float, std::size_t> CArray2DFloat;
typedef CArray2DBase<double, std::size_t> CArray2DDouble;
typedef CArray2DBase<int, std::size_t> CArray2DInt;


#endif