#ifndef CBOX_H
#define CBOX_H

template <class T> class CBox{
public:
  CBox() : mX1(0), mX2(0), mY1(0), mY2(0) {}
  CBox(T x1, T x2, T y1, T y2) : mX1(x1), mX2(x2), mY1(y1), mY2(y2) {}

  void ExtendBoundary(T ix, T iy){
    ExtendBoundaryX(ix);
    ExtendBoundaryY(iy);
  }

  void ExtendBoundaryX(T i){
    if (mX1 > i){
      mX1 = i;
    }
    if (mX2 < i){
      mX2 = i;
    }
  }

  void ExtendBoundaryY(T i){
    if (mY1 > i){
      mY1 = i;
    }
    if (mY2 < i){
      mY2 = i;
    }
  }

  T GetXSize() const {
    return mX2 - mX1;
  }

  T GetX1() const {
    return mX1;
  }

  T GetX2() const {
    return mX2;
  }

  T GetYSize() const {
    return mY2 - mY1;
  }

  T GetY1() const {
    return mY1;
  }

  T GetY2() const {
    return mY2;
  }

  bool IsDefined(){
    return (mX2 >= mX1) && (mY2 >= mY1);
  }

  bool IsInBox(T x, T y) {
    return (x >= mX1) && (x < mX2) && (y >= mY1) && (y < mY2);
  }

  void Reset(){
    mX1 = mX2 = mY1 = mY2 = 0;
  }

  void SetBoundary(T iX1, T iY1, T iX2, T iY2){
    SetBoundaryX(iX1, iX2);
    SetBoundaryY(iY1, iY2);
  }

  void SetBoundaryX(T Low, T High){
    mX1 = Low;
    mX2 = High;
  }

  void SetBoundaryY(T Low, T High){
    mY1 = Low;
    mY2 = High;
  }

  void SetSizeX(T DeltaX){
    mX2 = mX1 + DeltaX;
  }

  void SetSizeY(T DeltaY){
    mY2 = mY1 + DeltaY;
  }

  void SetX1(T i){
    mX1 = i;
  }

  void SetX2(T i){
    mX2 = i;
  }

  void SetY1(T i){
    mY1 = i;
  }

  void SetY2(T i){
    mY2 = i;
  }

protected:
  T mX1, mX2, mY1, mY2;
};

#endif