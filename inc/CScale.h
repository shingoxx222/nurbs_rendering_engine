#ifndef CSCALE_H
#define CSCALE_H

template <class T, class TIndex> class CScale{
public:
  
  T GetDelta() const{
    return mDelta;
  }

  T GetMin() const{
    return mMin;
  }

  void SetDelta(T Delta){
    mDelta = Delta;
    mDeltaInv = static_cast<T>(1.0) / Delta;
  }

  void SetMin(T Min){
    mMin = Min;
  }

  void SetScale(T Min, T Delta){
    SetDelta(Delta);
    SetMin(Min);
  }

  T IndexToScale(TIndex i){
    return mMin + i * mDelta;
  }

//  T IndexToScale(TIndex i){
//    return mMin + i * mDelta;
//  }

  T IndexToScaleCentered(TIndex i){
    return mMin + (i + 0.5) * mDelta;
  }

//  T IndexToScaleCentered(TIndex i){
//    return mMin + (i + 0.5) * mDelta;
//  }

  TIndex ScaleToIndex(T s){
    // returns index 0 if scale value is negative
    if (s - mMin >= 0.0){
      return static_cast<TIndex>((s - mMin)*mDeltaInv);
    }
    return 0;
  }

  TIndex ScaleToIndexClosest(T s){
    // returns index 0 if scale value is negative
    if (s - mMin >= 0.0){
      return static_cast<TIndex>((s - mMin)*mDeltaInv + 0.5);
    }
    return 0;
  }

protected:
  T mDelta;
  T mDeltaInv;
  T mMin;
};

#endif