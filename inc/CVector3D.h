 #ifndef CVECTOR3D_H
#define CVECTOR3D_H

// provides 3D-vectors and arithmetics for right handed coordinate system
// currently three data types are implemented: double, float, and long double (see typedef at end of header file).

#include <iostream>
#include <cmath>

template <class T> class CVector3DTemplate{

public:

  // x,y, and z coordinate
  T x;
  T y;
  T z;

  // constructors
  CVector3DTemplate<T>() : x(0), y(0), z(0) {}
  CVector3DTemplate<T>(T in_x, T in_y, T in_z) : x(in_x), y(in_y), z(in_z) {}

  // return absolute and absolute square of vector
  T Abs() const { return sqrt(x*x + y*y + z*z); }
  T AbsSqr() const { return x*x + y*y + z*z; }

  // normalizes the vector
  void Normalize(){ T norm(static_cast<T>(1.0) / sqrt(x*x + y*y + z*z)); x *= norm; y *= norm; z *= norm; }

  // Rotational direction given by right hand rule:
  // if thumb directs in positive axis direction, then the fingers
  // indicate the rotational direction
  // angle given in radiants

  // rotate using radiant unit parameter
  void RotateX(T rad){ RotateX(cos(rad), sin(rad)); }
  void RotateY(T rad){ RotateY(cos(rad), sin(rad)); }
  void RotateZ(T rad){ RotateZ(cos(rad), sin(rad)); }

  // rotate using pre-calculated sin(rad) and cos(rad) parameter
  void RotateX(T cosrad, T sinrad){
    T d;
    d = y*cosrad - z*sinrad;
    z = z*cosrad + y*sinrad;
    y = d;
  }

  void RotateY(T cosrad, T sinrad){
    T d;
    d = x*cosrad + z*sinrad;
    z = z*cosrad - x*sinrad;
    x = d;
  }

  void RotateZ(T cosrad, T sinrad){
    T d;
    d = x*cosrad - y*sinrad;
    y = y*cosrad + x*sinrad;
    x = d;
  }

  // sets vector components at once
  void Set(T in_x, T in_y, T in_z) { x = in_x; y = in_y; z = in_z; }

  // sets as zero vector
  void SetZero(){ x = y = z = 0; }

  // Assignment operators
  CVector3DTemplate<T>& operator+=(const CVector3DTemplate<T>& a) { x += a.x; y += a.y; z += a.z; return *this; }
  CVector3DTemplate<T>& operator-=(const CVector3DTemplate<T>& a) { x -= a.x; y -= a.y; z -= a.z; return *this; }
  CVector3DTemplate<T>& operator*=(T a) { x *= a; y *= a; z *= a; return *this; }
  CVector3DTemplate<T>& operator/=(T a) { const T d(static_cast<T>(1.0) / a);  x *= d; y *= d; z *= d; return *this; }

  // unitary minus operator
  CVector3DTemplate<T> operator-() const { return CVector3DTemplate<T>(-x, -y, -z); }

  // cast operator for conversion of vector of type T into vector of type T2
  template <class T2> operator CVector3DTemplate<T2>() { return CVector3DTemplate<T2>(static_cast<T2>(x), static_cast<T2>(y), static_cast<T2>(z)); }

};

// scalar product
template <class T> T operator*(const CVector3DTemplate<T>& a, const CVector3DTemplate<T>& b) { return a.x*b.x + a.y*b.y + a.z*b.z; }

// scalar multiplication and division
template <class T> CVector3DTemplate<T> operator*(T a, const CVector3DTemplate<T>& b) { return CVector3DTemplate<T>(a*b.x, a*b.y, a*b.z); }
template <class T> CVector3DTemplate<T> operator*(const CVector3DTemplate<T>& b, T a) { return CVector3DTemplate<T>(a*b.x, a*b.y, a*b.z); }
template <class T> CVector3DTemplate<T> operator/(const CVector3DTemplate<T>& b, T d) { T a(static_cast<T>(1.0) / d); return CVector3DTemplate<T>(a*b.x, a*b.y, a*b.z); }

// vector addition and subtraction
template <class T> CVector3DTemplate<T> operator+(const CVector3DTemplate<T>& a, const CVector3DTemplate<T>& b) { return CVector3DTemplate<T>(a.x + b.x, a.y + b.y, a.z + b.z); }
template <class T> CVector3DTemplate<T> operator-(const CVector3DTemplate<T>& a, const CVector3DTemplate<T>& b) { return CVector3DTemplate<T>(a.x - b.x, a.y - b.y, a.z - b.z); }

// vector cross product
template <class T> CVector3DTemplate<T> CrossProduct(const CVector3DTemplate<T>& a, const CVector3DTemplate<T>& b) { return CVector3DTemplate<T>(a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x); }

// stream operators
template <class T> std::ostream& operator<<(std::ostream& Stream, const CVector3DTemplate<T>& a) { Stream << "(" << a.x << ", " << a.y << ", " << a.z << ")"; return Stream; }

#ifdef FP_DEFAULT_TYPE
  typedef CVector3DTemplate<FP_DEFAULT_TYPE> CVector3D;
#else
  typedef CVector3DTemplate<double> CVector3D;
#endif

typedef CVector3DTemplate<double> CVector3DDouble;
typedef CVector3DTemplate<float> CVector3DFloat;
typedef CVector3DTemplate<long double> CVector3DLongDouble;

#endif