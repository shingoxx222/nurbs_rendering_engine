#ifndef CCOORDINATESYSTEM_H
#define CCOORDINATESYSTEM_H

#include <cmath>
#include "CVector3D.h"
#include "CMatrix3x3.h"

template <class T> class CCoordinateSystemTemplate{

public:

  //############################################################
  //  Constructors
  //############################################################

  // default constructor: initialize with same orientation and origin
  CCoordinateSystemTemplate(){}

  // constructor: takes rotation matrix and origin vector
  CCoordinateSystemTemplate(const CMatrix3x3Template<T>& RotMatrix, const CVector3DTemplate<T>& Origin){ mRotMatrix = RotMatrix; mOrigin = Origin; }

  //############################################################
  //  Get routines
  //############################################################

  // Get reference to origin vector
  CVector3DTemplate<T>& GetOrigin() { return mOrigin; }

  // Get reference to rotation matrix vector
  CMatrix3x3Template<T>& GetRotMatrix() { return mRotMatrix; }

  //############################################################
  //  Set routines
  //############################################################

  // resets rotations and translations with respect to reference system (i.e. results in same orientation and origin)
  void Reset(){ mOrigin.SetZero(); mRotMatrix.SetAsIdentity(); }

  // set coordinate origin vector in reference coordinates
  void SetOrigin(const CVector3DTemplate<T>& Origin){ mOrigin = Origin; }

  // set rotation matrix with respect to reference system
  void SetRotMatrix(const CMatrix3x3Template<T>& RotMatrix){ mRotMatrix = RotMatrix; }

  //############################################################
  //  Vector transformation routines
  //############################################################

  // transforms vector into reference system coordinates
  CVector3DTemplate<T> ToRefCoords(const CVector3DTemplate<T> v) const { return mRotMatrix*v + mOrigin; }

  // transforms vector given in reference coordinates into derived system coordinates
  CVector3DTemplate<T> ToDerCoords(const CVector3DTemplate<T> v) const { return mRotMatrix.TransposedMultipliedBy(v - mOrigin); }

  //############################################################
  // routines for coordinates system transformations
  //############################################################

  // rotate around axes of this (derived) system
  void RotateX(T rad){ mRotMatrix.RotateX(rad); }
  void RotateY(T rad){ mRotMatrix.RotateY(rad); }
  void RotateZ(T rad){ mRotMatrix.RotateZ(rad); }

  // rotate around axes of reference system (rotation matrix and origin vector needs to be rotated)
  // note that mOrigin is a vector always referring to the reference system, so the rotation routines does not have (or need) the suffix "ref"
  void RotateXRef(T rad){ const T cosrad(cos(rad)), sinrad(sin(rad));  mRotMatrix.RotateXRef(cosrad, sinrad); mOrigin.RotateX(cosrad, sinrad); }
  void RotateYRef(T rad){ const T cosrad(cos(rad)), sinrad(sin(rad));  mRotMatrix.RotateYRef(cosrad, sinrad); mOrigin.RotateY(cosrad, sinrad); }
  void RotateZRef(T rad){ const T cosrad(cos(rad)), sinrad(sin(rad));  mRotMatrix.RotateZRef(cosrad, sinrad); mOrigin.RotateZ(cosrad, sinrad); }
  void RotateXRef(T cosrad, T sinrad){ mRotMatrix.RotateXRef(cosrad, sinrad); mOrigin.RotateX(cosrad, sinrad); }
  void RotateYRef(T cosrad, T sinrad){ mRotMatrix.RotateYRef(cosrad, sinrad); mOrigin.RotateY(cosrad, sinrad); }
  void RotateZRef(T cosrad, T sinrad){ mRotMatrix.RotateZRef(cosrad, sinrad); mOrigin.RotateZ(cosrad, sinrad); }

  // translate along (derived system) axes
  void TranslateX(T delta){ Translate(CVector3DTemplate<T>(delta, 0, 0)); }
  void TranslateY(T delta){ Translate(CVector3DTemplate<T>(0, delta, 0)); }
  void TranslateZ(T delta){ Translate(CVector3DTemplate<T>(0, 0, delta)); }

  // translate coordinate system by a vector v (in derived system coordinates) with respect to reference system
  void Translate(const CVector3DTemplate<T>& v){ mOrigin += mRotMatrix*v; }

  // translate along reference axes
  void TranslateXRef(T delta){ mOrigin.x += delta; }
  void TranslateYRef(T delta){ mOrigin.y += delta; }
  void TranslateZRef(T delta){ mOrigin.z += delta; }

  // translate coordinate system by a vector v (in derived system coordinates) with respect to reference system
  void TranslateRef(const CVector3DTemplate<T>& v){ mOrigin += v; }

  //############################################################
  // other stuff
  //############################################################

  // link two coordinate systems to create a new derived system, stored in *NewDerivedSystemPtr.
  // ReferenceSystemPtr should point to the reference system of this coordinate system

  // The reference system of the new derived system is the reference system of the system ReferenceSystemPtr points to.
  bool LinkCoordinateSystems(CCoordinateSystemTemplate<T>* NewDerivedSystemPtr, CCoordinateSystemTemplate<T>* ReferenceSystemPtr){
    if (!NewDerivedSystemPtr){
      return false;
    }
    if (!ReferenceSystemPtr){
      // just copy the system
      *NewDerivedSystemPtr = *this;
      return true;
    }
    // generate roation matrix and origin for new system
    NewDerivedSystemPtr->SetOrigin(ReferenceSystemPtr->GetRotMatrix()*GetOrigin() + ReferenceSystemPtr->GetOrigin());
    NewDerivedSystemPtr->SetRotMatrix(ReferenceSystemPtr->GetRotMatrix()*GetRotMatrix());
    return true;
  }

  // cast operator for conversion of matrix data type T into data type T2
  template <class T2> operator CCoordinateSystemTemplate<T2>() { return CCoordinateSystemTemplate<T2>( static_cast<CMatrix3x3Template<T2>>(mRotMatrix), static_cast<CVector3DTemplate<T2>>(mOrigin)); }

  // reports unit vectors and origin in reference coordinates
  void ReportVectors(std::ostream& OStream){
    OStream << "ex = " << mRotMatrix * CVector3DTemplate<T>(1.0, 0, 0) << " ; ey = " << mRotMatrix * CVector3DTemplate<T>(0, 1.0, 0) <<
      " ; ez = " << mRotMatrix * CVector3DTemplate<T>(0, 0, 1.0) << " ; or = " << mOrigin << "\n";
  }

protected:

  CVector3DTemplate<T> mOrigin; // Coordinate origin in reference coordinates
  CMatrix3x3Template<T> mRotMatrix; // rotation matrix with respect to reference system

};

// stream operator
template <class T> std::ostream& operator<<(std::ostream& Stream, const CCoordinateSystemTemplate<T>& a) { Stream << a.GetRotMatrix() << a.GetOrigin(); return Stream; }

#ifdef FP_DEFAULT_TYPE
typedef CCoordinateSystemTemplate<FP_DEFAULT_TYPE> CCoordinateSystem;
#else
typedef CCoordinateSystemTemplate<double> CCoordinateSystem;
#endif

typedef CCoordinateSystemTemplate<double> CCoordinateSystemDouble;
typedef CCoordinateSystemTemplate<float> CCoordinateSystemFloat;
typedef CCoordinateSystemTemplate<long double> CCoordinateSystemLongDouble;

#endif