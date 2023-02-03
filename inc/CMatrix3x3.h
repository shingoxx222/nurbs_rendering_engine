#ifndef CMATRIX3x3_H
#define CMATRIX3x3_H

#include <cmath>
#include <iostream>
#include "CVector3D.h"

template <class T> class CMatrix3x3Template{

public:

  // matrix coefficients
  //x-  y-  z- columns
  T xx, xy, xz;    // x-row
  T yx, yy, yz;    // y-row
  T zx, zy, zz;    // z-row

  // constructor (1) no arguements, initializes by with identity (unit) matrix
  CMatrix3x3Template(){ xx = yy = zz = static_cast<T>(1.0); xy = xz = yx = yz = zx = zy = 0; }

  // constructor (2) specification of 9 matrix elements
  CMatrix3x3Template(T xx_in, T xy_in, T xz_in, T yx_in, T yy_in, T yz_in, T zx_in, T zy_in, T zz_in){
    xx = xx_in; xy = xy_in; xz = xz_in;
    yx = yx_in; yy = yy_in; yz = yz_in;
    zx = zx_in; zy = zy_in; zz = zz_in;
  }

  // constructor (3) specification by three vectors representing the matrix columns: xCol=(xx, yx, zx); yCol=(xy, yy, zy); zCol=(xz, yz, zz)
  CMatrix3x3Template(const CVector3DTemplate<T>& xCol, const CVector3DTemplate<T>& yCol, const CVector3DTemplate<T>& zCol){
    xx = xCol.x; xy = yCol.x; xz = zCol.x;
    yx = xCol.y; yy = yCol.y; yz = zCol.y;
    zx = xCol.z; zy = yCol.z; zz = zCol.z;
  }

  // Get columns of matrix in CVector type
  CVector3DTemplate<T> GetColumnX() const { return CVector3DTemplate<T>(xx, yx, zx); }
  CVector3DTemplate<T> GetColumnY() const { return CVector3DTemplate<T>(xy, yy, zy); }
  CVector3DTemplate<T> GetColumnZ() const { return CVector3DTemplate<T>(xz, yz, zz); }

  // Get rows of matrix in CVector type
  CVector3DTemplate<T> GetRowX() const { return CVector3DTemplate<T>(xx, xy, xz); }
  CVector3DTemplate<T> GetRowY() const { return CVector3DTemplate<T>(yx, yy, yz); }
  CVector3DTemplate<T> GetRowZ() const { return CVector3DTemplate<T>(zx, zy, zz); }

  // Get transpose of the matrix (original matrix is conserved).
  CMatrix3x3Template<T> GetTranspose() const { return CMatrix3x3Template<T>(xx, yx, zx, xy, yy, zy, xz, yz, zz); }

  // Following routines multiplicate current matrix M
  // with a rotation matrix R to get the new matrix M' = R * M,
  // i.e. any vector v multiplied with M' results into a vector rotated
  // with respect to the axes of the reference coordinate system.
  // Rotational direction given by right hand rule:
  // if thumb directs in positive axis direction, then the fingers
  // indicate the rotational direction

  // rotate using radiant unit parameter
  void RotateXRef(T rad){ RotateXRef(cos(rad), sin(rad)); }
  void RotateYRef(T rad){ RotateYRef(cos(rad), sin(rad)); }
  void RotateZRef(T rad){ RotateZRef(cos(rad), sin(rad)); }

  // rotate using pre-calculated sin(rad) and cos(rad) parameter
  void RotateXRef(T cosrad, T sinrad){
    T d;
    d = yx * cosrad - zx * sinrad;
    zx = zx * cosrad + yx * sinrad;
    yx = d;
    d = yy * cosrad - zy * sinrad;
    zy = zy * cosrad + yy * sinrad;
    yy = d;
    d = yz * cosrad - zz * sinrad;
    zz = zz * cosrad + yz * sinrad;
    yz = d;
  }

  void RotateYRef(T cosrad, T sinrad){
    T d;
    d = xx * cosrad + zx * sinrad;
    zx = zx * cosrad - xx * sinrad;
    xx = d;
    d = xy * cosrad + zy * sinrad;
    zy = zy * cosrad - xy * sinrad;
    xy = d;
    d = xz * cosrad + zz * sinrad;
    zz = zz * cosrad - xz * sinrad;
    xz = d;
  }

  void RotateZRef(T cosrad, T sinrad){
    T d;
    d = xx * cosrad - yx * sinrad;
    yx = yx * cosrad + xx * sinrad;
    xx = d;
    d = xy * cosrad - yy * sinrad;
    yy = yy * cosrad + xy * sinrad;
    xy = d;
    d = xz * cosrad - yz * sinrad;
    yz = yz * cosrad + xz * sinrad;
    xz = d;
  }

  // Following routines multiplicate a rotation matrix R
  // with the current matrix M to get the new matrix M' = M * R,
  // i.e. any vector v multiplied with M' results into a vector rotated
  // with respect to the derived(!) system:
  // M' = M * R = M * R * M^-1 * M = (M*R*M^-1) * M
  // --> (M*R*m^-1) can be seen as reversing the original rotation (M^-1),
  // then rotation (R) around the axes (after reversion, reference system and
  // derived system have same orientation), then redoing the original rotation (M).
  // Rotational direction given by right hand rule:
  // if thumb directs in positive axis direction, then the fingers
  // indicate the rotational direction

  // rotate using radiant unit parameter
  void RotateX(T rad){ RotateX(cos(rad), sin(rad)); }
  void RotateY(T rad){ RotateY(cos(rad), sin(rad)); }
  void RotateZ(T rad){ RotateZ(cos(rad), sin(rad)); }

  // rotate using pre-calculated sin(rad) and cos(rad) parameter
  void RotateX(T cosrad, T sinrad){
    T d;
    d = xy * cosrad + xz * sinrad;
    xz = xz * cosrad - xy * sinrad;
    xy = d;
    d = yy * cosrad + yz * sinrad;
    yz = yz * cosrad - yy * sinrad;
    yy = d;
    d = zy * cosrad + zz * sinrad;
    zz = zz * cosrad - zy * sinrad;
    zy = d;
  }

  void RotateY(T cosrad, T sinrad){
    T d;
    d = xx * cosrad - xz * sinrad;
    xz = xz * cosrad + xx * sinrad;
    xx = d;
    d = yx * cosrad - yz * sinrad;
    yz = yz * cosrad + yx * sinrad;
    yx = d;
    d = zx * cosrad - zz * sinrad;
    zz = zz * cosrad + zx * sinrad;
    zx = d;
  }

  void RotateZ(T cosrad, T sinrad){
    T d;
    d = xx * cosrad + xy * sinrad;
    xy = xy * cosrad - xx * sinrad;
    xx = d;
    d = yx * cosrad + yy * sinrad;
    yy = yy * cosrad - yx * sinrad;
    yx = d;
    d = zx * cosrad + zy * sinrad;
    zy = zy * cosrad - zx * sinrad;
    zx = d;
  }

  // Set all matrix elements at once
  void Set(T xx_in, T xy_in, T xz_in, T yx_in, T yy_in, T yz_in, T zx_in, T zy_in, T zz_in){
    xx = xx_in; xy = xy_in; xz = xz_in;
    yx = yx_in; yy = yy_in; yz = yz_in;
    zx = zx_in; zy = zy_in; zz = zz_in;
  }

  // sets matrix elements for identity (unit) matrix
  void SetAsIdentity() { xx = yy = zz = static_cast<T>(1.0); xy = xz = yx = yz = zx = zy = 0; }

  // sets the whole matrix to zero
  void SetAsZero() { xx = xy = xz = yx = yy = yz = zx = zy = zz = 0; }

  // set columns of matrix
  void SetColumns(const CVector3DTemplate<T>& xCol, const CVector3DTemplate<T>& yCol, const CVector3DTemplate<T>& zCol){
    xx = xCol.x; xy = yCol.x; xz = zCol.x;
    yx = xCol.y; yy = yCol.y; yz = zCol.y;
    zx = xCol.z; zy = yCol.z; zz = zCol.z;
  }

  void SetColumnX(const CVector3DTemplate<T>& xCol){
    xx = xCol.x; yx = xCol.y; zx = xCol.z;
  }

  void SetColumnY(const CVector3DTemplate<T>& yCol){
    xy = yCol.x; yy = yCol.y; zy = yCol.z;
  }

  void SetColumnZ(const CVector3DTemplate<T>& zCol){
    xz = zCol.x; yz = zCol.y; zz = zCol.z;
  }

  // set rows of matrix
  void SetRows(const CVector3DTemplate<T>& xRow, const CVector3DTemplate<T>& yRow, const CVector3DTemplate<T>& zRow){
    xx = xRow.x; xy = xRow.y; xz = xRow.z;
    yx = yRow.x; yy = yRow.y; yz = yRow.z;

    zx = zRow.x; zy 
      = zRow.y; zz = zRow.z;
  }

  void SetRowX(const CVector3DTemplate<T>& xRow){
    xx = xRow.x; xy = xRow.y; xz = xRow.z;
  }

  void SetRowY(const CVector3DTemplate<T>& yRow){
    yx = yRow.x; yy = yRow.y; yz = yRow.z;
  }

  void SetRowZ(const CVector3DTemplate<T>& zRow){
    zx = zRow.x; zy = zRow.y; zz = zRow.z;
  }

  // transposes the matrix
  void Transpose() { T d(xy); xy = yx; yx = d; d = xz; xz = zx; zx = d; d = yz; yz = zy; zy = d; }

  // returns result of multiplication of transposed matrix with vector v
  CVector3DTemplate<T> TransposedMultipliedBy(CVector3DTemplate<T> v) const {
    return CVector3DTemplate<T>(xx * v.x + yx * v.y + zx * v.z, xy * v.x + yy * v.y + zy * v.z, xz * v.x + yz * v.y + zz * v.z);
  }

  // Assignment operators

  CMatrix3x3Template<T>& operator+=(const CMatrix3x3Template<T>& a) {
    xx += a.xx; xy += a.xy; xz += a.xz;
    yx += a.yx; yy += a.yy; yz += a.yz;
    zx += a.zx; zy += a.zy; zz += a.zz;
    return *this;
  }

  CMatrix3x3Template<T>& operator-=(const CMatrix3x3Template<T>& a) {
    xx -= a.xx; xy -= a.xy; xz -= a.xz;
    yx -= a.yx; yy -= a.yy; yz -= a.yz;
    zx -= a.zx; zy -= a.zy; zz -= a.zz;
    return *this;
  }

  CMatrix3x3Template<T>& operator*=(T a) {
    xx *= a; xy *= a; xz *= a;
    yx *= a; yy *= a; yz *= a;
    zx *= a; zy *= a; zz *= a;
    return *this;
  }

  CMatrix3x3Template<T>& operator/=(T d) {
    T a(static_cast<T>(1.0) / d);
    xx *= a; xy *= a; xz *= a;
    yx *= a; yy *= a; yz *= a;
    zx *= a; zy *= a; zz *= a;
    return *this;
  }

  // cast operator for conversion of matrix data type T into data type T2
  template <class T2> operator CMatrix3x3Template<T2>() { return CMatrix3x3Template<T2>(
    static_cast<T2>(xx), static_cast<T2>(xy), static_cast<T2>(xz),
    static_cast<T2>(yx), static_cast<T2>(yy), static_cast<T2>(yz),
    static_cast<T2>(zx), static_cast<T2>(zy), static_cast<T2>(zz) );
  }
};

// multiplication of matrix with vector
template <class T> CVector3DTemplate<T> operator*(const CMatrix3x3Template<T>& a, const CVector3DTemplate<T>& b){
  return CVector3DTemplate<T>( a.xx * b.x + a.xy * b.y + a.xz * b.z, a.yx * b.x + a.yy * b.y + a.yz * b.z, a.zx * b.x + a.zy * b.y + a.zz * b.z);
}

// multiplication of two matrices
template <class T> CMatrix3x3Template<T> operator*(const CMatrix3x3Template<T>& a, const CMatrix3x3Template<T>& b){
  return CMatrix3x3(
    a.xx * b.xx + a.xy * b.yx + a.xz * b.zx,
    a.xx * b.xy + a.xy * b.yy + a.xz * b.zy,
    a.xx * b.xz + a.xy * b.yz + a.xz * b.zz,
    a.yx * b.xx + a.yy * b.yx + a.yz * b.zx,
    a.yx * b.xy + a.yy * b.yy + a.yz * b.zy,
    a.yx * b.xz + a.yy * b.yz + a.yz * b.zz,
    a.zx * b.xx + a.zy * b.yx + a.zz * b.zx,
    a.zx * b.xy + a.zy * b.yy + a.zz * b.zy,
    a.zx * b.xz + a.zy * b.yz + a.zz * b.zz);
}

// scalar multiplication and division

template <class T> CMatrix3x3Template<T> operator*(T a, const CMatrix3x3Template<T>& b) {
  return CMatrix3x3Template<T>(a*b.xx, a*b.xy, a*b.xz, a*b.yx, a*b.yy, a*b.yz, a*b.zx, a*b.zy, a*b.zz);
}

template <class T> CMatrix3x3Template<T> operator*(const CMatrix3x3Template<T>& b, T a) {
  return CMatrix3x3Template<T>(a*b.xx, a*b.xy, a*b.xz, a*b.yx, a*b.yy, a*b.yz, a*b.zx, a*b.zy, a*b.zz);
}

template <class T> CMatrix3x3Template<T> operator/(const CMatrix3x3Template<T>& b, T d) {
  T a(static_cast<T>(1.0) / d);
  return CMatrix3x3Template<T>(a*b.xx, a*b.xy, a*b.xz, a*b.yx, a*b.yy, a*b.yz, a*b.zx, a*b.zy, a*b.zz);
}

// matrix addition operator
template <class T> CMatrix3x3Template<T> operator+(const CMatrix3x3Template<T>& a, const CMatrix3x3Template<T>& b) {
  return CMatrix3x3Template<T>(
    a.xx + b.xx, a.xy + b.xy, a.xz + b.xz,
    a.yx + b.yx, a.yy + b.yy, a.yz + b.yz,
    a.zx + b.zx, a.zy + b.zy, a.zz + b.zz );
}

// matrix subtraction operator
template <class T> CMatrix3x3Template<T> operator-(const CMatrix3x3Template<T>& a, const CMatrix3x3Template<T>& b) {
  return CMatrix3x3Template<T>(
    a.xx - b.xx, a.xy - b.xy, a.xz - b.xz,
    a.yx - b.yx, a.yy - b.yy, a.yz - b.yz,
    a.zx - b.zx, a.zy - b.zy, a.zz - b.zz);
}

// stream operators
template <class T> std::ostream& operator<<(std::ostream& Stream, const CMatrix3x3Template<T>& a) {
  Stream << "(" << a.xx << ", " << a.xy << ", " << a.xz << "; " << a.yx << ", " << a.yy << ", " << a.yz << "; " << a.zx << ", " << a.zy << ", " << a.zz << ")";
  return Stream;
}

#ifdef FP_DEFAULT_TYPE
typedef CMatrix3x3Template<FP_DEFAULT_TYPE> CMatrix3x3;
#else
typedef CMatrix3x3Template<double> CMatrix3x3;
#endif

typedef CMatrix3x3Template<double> CMatrix3x3Double;
typedef CMatrix3x3Template<float> CMatrix3x3Float;
typedef CMatrix3x3Template<long double> CMatrix3x3LongDouble;

#endif