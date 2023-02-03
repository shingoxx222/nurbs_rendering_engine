#ifndef CARRAY_H
#define CARRAY_H

///////////////////////////////////////////////////////////
// 170907 quick hack of a CArray version compilable under CUDA
////////////////////////////////////////////////////////

template <class T, class TScale>
class CArray{

public:

  CArray(){
    CArrayInit();
  }

  CArray(std::size_t n){
    CArrayInit();
    Init(n);
  }

  ~CArray(){
    DeleteArray();
  }

  void CArrayInit(){
    mArray= 0;
    mNValues= 0;
    mMin= 0.0;
    mDelta= mDeltaInv= 1.0;
  }

  inline T& operator[](std::size_t i){
    return mArray[i];
  }

  void DeleteArray(){
    if( mArray ){
      delete[] mArray;
      mArray= 0;
      mNValues= 0;
    }
  }

  bool Init(std::size_t n){
    if (n == mNValues){
      return true;
    }
    DeleteArray();
    if( n==0 ){
      return true;
    }
    mArray= new T[n];
    if( mArray ){
      mNValues= n;
      return true;
    }
    return false;
  }

  void AddScaledArray(T Scalar, CArray<T, TScale>& Array){
    std::size_t i;
    for( i=0; i<mNValues; i++){
      mArray[i] += Scalar * Array[i];
    }
  }
  
  void ScaleValues( T t ){
    std::size_t i;
    for( i=0; i<mNValues; i++){
      mArray[i] *= t;
    }
  }

  inline bool IsInRangeX(std::size_t i){ return i < mNValues; }
  inline bool IsInRangeX(TScale i){ TScale d((i - mMin) * mDeltaInv); return (d < mNValues) && (d>=0.0);}

  inline std::size_t NValues(){ return mNValues;}

  inline TScale Delta(){ return mDelta; }
  inline TScale DeltaInv(){ return mDeltaInv; }
  inline TScale Max(){ return ToX(mNValues); }
  inline TScale Min(){ return mMin; }

  inline T* Pointer(){ return mArray; }

  void SetAll(T Value){
    std::size_t i;
    for( i=0; i<mNValues; i++ ){
      mArray[i]= Value;
    }
  }

  inline void SetDelta(TScale Delta){
    mDelta= Delta;
    mDeltaInv= static_cast<T>(1.0) / Delta;
  }

  inline void SetMin(TScale Min){
    mMin= Min;
  }

  inline void SetScale(TScale Min, TScale Delta){
    SetMin(Min);
    SetDelta(Delta);
  }

  inline TScale ToX(std::size_t i){ return mMin + i * mDelta; }
  inline TScale ToX(TScale i){ return mMin + i * mDelta; }
  inline TScale ToXCentered(std::size_t i){ return mMin + (i + 0.5) * mDelta; }

  inline std::size_t ToI(TScale x){
    // keep care: undefined result if x is out of range
    return static_cast<std::size_t>((x - mMin) * mDeltaInv);
  }

  inline T Value(std::size_t i){ return mArray[i]; }

  inline T Value(TScale x){
    TScale ix= (x - mMin) * mDeltaInv;
    std::size_t i= static_cast<std::size_t>(ix);
    if( ix > static_cast<std::size_t>(mNValues - 1)){
      i= mNValues - 2;
    }
    if( ix < 0.0 ){
      i=0;
    }
    return mArray[i] + (mArray[i+1] - mArray[i])*static_cast<T>(ix-i);
  }

protected:

  T *mArray;
  TScale mDelta, mDeltaInv;
  TScale mMin;
  std::size_t mNValues;
};

typedef CArray<float, double> CArrayFloat;
typedef CArray<double, double> CArrayDouble;
typedef CArray<int, double> CArrayInt;

#endif