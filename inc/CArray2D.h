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


template <class T, class TScale, class TIndex> class CArray2DWithScale : public CArray2DBase < T, TIndex > {
public:
  // assignment operator
  CArray2DWithScale<T, TScale, TIndex>& operator=(CArray2DWithScale<T, TScale, TIndex>& Source) {
    if (this != &Source) {
      CArray2DBase::operator=(Source);
      x.SetScale(Source.x.GetMin(), Source.x.GetDelta());
      y.SetScale(Source.y.GetMin(), Source.y.GetDelta());
    }
    return *this;
  }

  template <class T2, class TScale2, class TIndex2> CArray2DWithScale<T, TScale, TIndex>& operator=(CArray2DWithScale<T2, TScale2, TIndex2>& Source){
    if (this != reinterpret_cast<CArray2DWithScale<T, TScale, TIndex>*>(&Source)){
      CArray2DBase::operator=(Source);
      x.SetScale(Source.x.GetMin(), Source.x.GetDelta());
      y.SetScale(Source.y.GetMin(), Source.y.GetDelta());
    }
    return *this;
  }

  void BoxFullField(CBox<TIndex>& Box) {
    CArray2DBase<T,TIndex>::BoxFullField(Box);
  }

  void BoxFullField(CBox<TScale>& Box){
    Box.SetBoundary(XMin(), YMin(), XMax(), YMax());
  }

  void BoxIndexToScale(CBox<TIndex>& IndexBox, CBox<TScale>& ScaleBox){
    ScaleBox.SetBoundary(x.IndexToScale(IndexBox.GetX1()), y.IndexToScale(IndexBox.GetY1()), x.IndexToScale(IndexBox.GetX2()), y.IndexToScale(IndexBox.GetY2()));
  }

  void BoxScaleToIndex(CBox<TScale>& ScaleBox, CBox<TIndex>& IndexBox){
    IndexBox.SetBoundary(x.ScaleToIndex(ScaleBox.GetX1()), y.ScaleToIndex(ScaleBox.GetY1()), x.ScaleToIndex(ScaleBox.GetX2()), y.ScaleToIndex(ScaleBox.GetY2()));
  }

  void BoxScaleToIndexClosest(CBox<TScale>& ScaleBox, CBox<TIndex>& IndexBox){
    IndexBox.SetBoundary(x.ScaleToIndexClosest(ScaleBox.GetX1()), y.ScaleToIndexClosest(ScaleBox.GetY1()), x.ScaleToIndexClosest(ScaleBox.GetX2()), y.ScaleToIndexClosest(ScaleBox.GetY2()));
  }

  TScale XDelta(){ return x.GetDelta(); }
  TScale XMax(){ return x.GetMin() + mNValuesX * x.GetDelta(); }
  TScale XMin(){ return x.GetMin(); }
  TScale YDelta(){ return y.GetDelta(); }
  TScale YMax(){ return y.GetMin() + mNValuesY * y.GetDelta(); }
  TScale YMin(){ return y.GetMin(); }

  CScale<TScale, TIndex> x;
  CScale<TScale, TIndex> y;
};

template <class T1, class TScale1, class TIndex1, class T2, class TScale2, class TIndex2> class CArray2DSampler{
// Sample from Source (index1) to destination (index2)

public:
  CArray2DSampler() : mDestinationPtr(0), mSourcePtr(0){}
  void SetDestinationPtr(CArray2DWithScale<T2, TScale2, TIndex2>* DestinationPtr){ mDestinationPtr = DestinationPtr; }
  void SetSourcePtr     (CArray2DWithScale<T1, TScale1, TIndex1>* SourcePtr){ mSourcePtr = SourcePtr; }

  // sample another 2D array considering the scales of x and y
  bool Sample(){
    CBox<TIndex2> DestIBox;
    CBox<TIndex1> SourceIBox;
    CBox<TScale2> ScaleBox;
    T1 *SPtr, *SLastXPtr, *SLastYPtr;
    T2 *DPtr;
    TIndex1 Sdx, Sdy;
    TIndex2 Ddy;

    if (!Check()){
      return false;
    }
    mDestinationPtr->SetAll(0);
    if ((mSourcePtr->x.GetDelta() >= mDestinationPtr->x.GetDelta() * 0.99999) && (mSourcePtr->x.GetDelta() <= mDestinationPtr->x.GetDelta() * 1.00001) &&
      (mSourcePtr->y.GetDelta() >= mDestinationPtr->y.GetDelta() * 0.99999) && (mSourcePtr->y.GetDelta() <= mDestinationPtr->y.GetDelta() * 1.00001)){
      // Source and Destination have same pitch
      // make a one-to-one copy of a matching pixel field
      mDestinationPtr->BoxFullField(DestIBox);
      mSourcePtr->BoxFullField(SourceIBox);
      // check if lower corner of source field is covered by destination field
      if (mDestinationPtr->XMin() < mSourcePtr->XMin()){
        // no it's not for x
        DestIBox.SetX1(mDestinationPtr->x.ScaleToIndexClosest(mSourcePtr->XMin()));
      }
      else{
        SourceIBox.SetX1(mSourcePtr->x.ScaleToIndexClosest(mDestinationPtr->XMin()));
      }
      if (mDestinationPtr->YMin() < mSourcePtr->YMin()){
        // no it's not for y
        DestIBox.SetY1(mDestinationPtr->y.ScaleToIndexClosest(mSourcePtr->YMin()));
      }
      else{
        SourceIBox.SetY1(mSourcePtr->y.ScaleToIndexClosest(mDestinationPtr->YMin()));
      }
      // find upper corner of source index box
      SourceIBox.SetX2(SourceIBox.GetX1() + DestIBox.GetXSize());
      SourceIBox.SetY2(SourceIBox.GetY1() + DestIBox.GetYSize());
      mSourcePtr->BoxCropToFit(SourceIBox);
      if (!DestIBox.IsDefined() || !SourceIBox.IsDefined()){
        // no overlap of fields
        return true;
      }
      // now, it is confirmed that an overlapping area exist; the source index box might due to cropping be smaller than the destination index box.
      // Simply take the source box size for copying.
      Sdx = SourceIBox.GetXSize();
      Sdy = mSourcePtr->GetNValuesX() - Sdx;
      Ddy = mDestinationPtr->GetNValuesX() - Sdx;
      SPtr = mSourcePtr->GetPtr() + SourceIBox.GetY1() * mSourcePtr->GetNValuesX() + SourceIBox.GetX1();
      SLastYPtr = SPtr + SourceIBox.GetYSize() * mSourcePtr->GetNValuesX();
      DPtr =   mDestinationPtr->GetPtr() +   DestIBox.GetY1() *   mDestinationPtr->GetNValuesX() +   DestIBox.GetX1();
      while (SPtr < SLastYPtr){
        SLastXPtr = SPtr + Sdx;
        while (SPtr < SLastXPtr){
          *DPtr++ = *SPtr++;
        }
        SPtr += Sdy;
        DPtr += Ddy;
      }
    }
    else {
      // pitches doesn't fit each other; perform a re-sampling
      // find ROI in destination box
      DestIBox.SetX1(mDestinationPtr->x.ScaleToIndex(mSourcePtr->XMin()));
      DestIBox.SetY1(mDestinationPtr->y.ScaleToIndex(mSourcePtr->YMin()));
      DestIBox.SetX2(mDestinationPtr->x.ScaleToIndex(mSourcePtr->XMax())+1);
      DestIBox.SetY2(mDestinationPtr->y.ScaleToIndex(mSourcePtr->YMax())+1);
      mDestinationPtr->BoxCropToFit(DestIBox);
      if (!DestIBox.IsDefined()) {
        // no overlap
        return true;
      }
      // init help arrays for sampling
      if (!mSampledNPixelX.Init(mDestinationPtr->GetNValuesX()) || !mSampledNPixelY.Init(mDestinationPtr->GetNValuesY()) ||
        !mWeightX.Init(mDestinationPtr->GetNValuesX() + mSourcePtr->GetNValuesX()) || !mWeightY.Init(mDestinationPtr->GetNValuesY() + mSourcePtr->GetNValuesY())) {
        // exception routine still to be impleneted
        return false;
      }
      TScale1 PitchSXInv(1.0 / mSourcePtr->XDelta());
      TScale1 PitchSYInv(1.0 / mSourcePtr->YDelta());
      TScale1 ssx1, ssx2, ssy1, ssy2;
      TIndex1 isx1, isx2, isy, isy1, isy2;
      TIndex1 WeightIndex;
      TIndex2 idx;
      TIndex2 idy;
      // find initial start pixels in soure array
      ssx2 = mDestinationPtr->x.IndexToScale(DestIBox.GetX1());  // = lowest sampled x-value in source array
      if (ssx2 > mSourcePtr->XMin()) {
        isx2 = mSourcePtr->x.ScaleToIndex(ssx2);
      }
      else {
        ssx2 = mSourcePtr->XMin();
        isx2 = 0;
      }
      if (isx2 >= mSourcePtr->GetNValuesX()) {
        // this might happen if rounding errors occured;
        // --> no overlap
        return true;
      }
      ssy2 = mDestinationPtr->y.IndexToScale(DestIBox.GetY1());  // = lowest sampled y-value in source array
      if (ssy2 > mSourcePtr->YMin()) {
        isy2 = mSourcePtr->y.ScaleToIndex(ssy2);
      }
      else {
        ssy2 = mSourcePtr->YMin();
        isy2 = 0;
      }
      if (isy2 >= mSourcePtr->GetNValuesY()) {
        // this might happen if rounding errors occured;
        // --> no overlap
        return true;
      }
      mSampledStartX = isx2;
      mSampledStartY = isy2;
      WeightIndex = 0;
      for (idx = DestIBox.GetX1(); idx < DestIBox.GetX2(); idx++) {
        // isx1 and isx2 are the boundary indices in the source field of pixel idx of the destination field
        isx1 = isx2;
        ssx1 = ssx2;
        ssx2 = mDestinationPtr->x.IndexToScale(idx + 1);
        isx2 = mSourcePtr->x.ScaleToIndex(ssx2);
        if (isx2 >= mSourcePtr->GetNValuesX()) {
          isx2 = mSourcePtr->GetNValuesX() - 1;
          ssx2 = mSourcePtr->XMax();
        }
        mSampledNPixelX[idx] = isx2 - isx1 + 1; // = number of pixels covered in source field
        if (isx1 == isx2) {
          // only one source pixel sampled
          mWeightX[WeightIndex++] = (ssx2 - ssx1) * PitchSXInv;
        }
        else {
          mWeightX[WeightIndex++] = (mSourcePtr->x.IndexToScale(isx1+1) - ssx1) * PitchSXInv;
          for (isx1++; isx1 < isx2; isx1++) {
            mWeightX[WeightIndex++] = 1.0;
          }
          mWeightX[WeightIndex++] = (ssx2 - mSourcePtr->x.IndexToScale(isx2))* PitchSXInv;
        }
      }
      WeightIndex = 0;
      for (idy = DestIBox.GetY1(); idy < DestIBox.GetY2(); idy++) {
        isy1 = isy2;
        ssy1 = ssy2;
        ssy2 = mDestinationPtr->y.IndexToScale(idy + 1);
        isy2 = mSourcePtr->y.ScaleToIndex(ssy2);
        if (isy2 >= mSourcePtr->GetNValuesY()) {
          isy2 = mSourcePtr->GetNValuesY() - 1;
          ssy2 = mSourcePtr->YMax();
        }
        mSampledNPixelY[idy] = isy2 - isy1 + 1; // = number of pixels covered in source field
        if (isy1 == isy2) {
          // only one source pixel sampled
          mWeightY[WeightIndex++] = (ssy2 - ssy1) * PitchSYInv;
        }
        else {
          mWeightY[WeightIndex++] = (mSourcePtr->y.IndexToScale(isy1 + 1) - ssy1) * PitchSYInv;
          for (isy1++; isy1 < isy2; isy1++) {
            mWeightY[WeightIndex++] = 1.0;
          }
          mWeightY[WeightIndex++] = (ssy2 - mSourcePtr->y.IndexToScale(isy2))* PitchSYInv;
        }
      }
      // now sample the source array and store result in destination array
      T2* DPtrStartLine;
      T2 Sum;
      TScale1* WeightXPtr;
      TScale1* WeightYPtr;
      isy = mSampledStartY;
      WeightYPtr = mWeightY.Pointer();
      for (idy = DestIBox.GetY1(); idy < DestIBox.GetY2(); idy++) {
        // DPtrStartLine points to the first destination array element of a line
        DPtrStartLine = mDestinationPtr->GetPtr() + idy * mDestinationPtr->GetNValuesX() + DestIBox.GetX1();
        for (isy1 = 0; isy1 < mSampledNPixelY[idy]; isy1++) {
          DPtr = DPtrStartLine;
          SPtr = mSourcePtr->GetPtr() + isy * mSourcePtr->GetNValuesX() + mSampledStartX;
          WeightXPtr = mWeightX.Pointer();
          for (idx = DestIBox.GetX1(); idx < DestIBox.GetX2(); idx++) {
            Sum = 0.0;
            for (isx1=0; isx1 < mSampledNPixelX[idx]; isx1++){
              Sum += *SPtr++ * static_cast<T1>(*WeightXPtr++ * *WeightYPtr);
            }
            *DPtr++ += Sum;
            --SPtr;
          }
          ++isy;
          ++WeightYPtr;
        }
        --isy;
      }
    }
    return true;
  }


protected:
  bool Check(){
    return mDestinationPtr && mSourcePtr;
  }

  /*
  bool Init(){
    size_t ix, iy, nx, ny;
    if (!Check() || !mSourceIndexX.Init(mSourcePtr->NValuesX()) || !mSourceIndexY.Init(mSourcePtr->NValuesY()) ||
      !mSourceOffsetX.Init(mSourcePtr->NValuesX()) || !mSourceOffsetY.Init(mSourcePtr->NValuesY())){
      return false;
    }
  }
  */

  CArray2DWithScale<T2, TScale2, TIndex2>* mDestinationPtr;
  CArray2DWithScale<T1, TScale1, TIndex1>* mSourcePtr;
  CArray<TIndex1, TScale1> mSampledNPixelX;
  CArray<TIndex1, TScale1> mSampledNPixelY;
  TIndex1 mSampledStartX;
  TIndex1 mSampledStartY;
  CArray<TScale1, TScale1> mWeightX;
  CArray<TScale1, TScale1> mWeightY;
  /*

  CArray<size_t, FP> mSourceIndexX, mSourceIndexY;
  CArray<FP, FP> mSourceOffsetX, mSourceIndexY;
  */
};

typedef CArray2DBase<float, std::size_t> CArray2DFloat;
typedef CArray2DBase<double, std::size_t> CArray2DDouble;
typedef CArray2DBase<int, std::size_t> CArray2DInt;


#endif