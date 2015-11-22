#ifndef MATRIX3_H
#define MATRIX3_H

#include <iostream>
#include "Vector3.h"

namespace Math
{
  class Matrix3
  {
  public:
    Matrix3 ();

    Matrix3 (float rx, float ry, float rz, float ux, float uy, float uz,
	     float bx, float by, float bz);

    Matrix3 (const Vector3& right, const Vector3& up, const Vector3& back);

    Matrix3 (const Vector3& right, const Vector3& up, bool makeOrthonormal =
		 false);

    void
    setToIdentity ();

    void
    setToZero ();

    const float*
    data () const;

    void
    setRight (const Vector3& right);

    Vector3
    getRight () const;

    void
    setUp (const Vector3& up);

    Vector3
    getUp () const;

    void
    setBack (const Vector3& back);

    Vector3
    getBack () const;

    void
    setForward (const Vector3& forward);

    Vector3
    getForward () const;

    void
    invertRotation ();

    void
    invert ();

    float
    determinant () const;

    void
    transpose ();

    void
    orthonormalize ();

    void
    setToScale (float scale);

    void
    setToScale (float scaleX, float scaleY, float scaleZ);

    void
    setToShearXByYz (float shearY, float shearZ);

    void
    setToShearYByXz (float shearX, float shearZ);

    void
    setToShearZByXy (float shearX, float shearY);

    void
    setToRotationX (float angleDegrees);

    void
    setToRotationY (float angleDegrees);

    void
    setToRotationZ (float angleDegrees);

    void
    setFromAngleAxis (float angleDegrees, const Vector3& axis);

    void
    negate ();

    Vector3
    transform (const Vector3& point) const;

    Matrix3&
    operator+= (const Matrix3& m);

    Matrix3&
    operator-= (const Matrix3& m);

    Matrix3&
    operator*= (float scalar);

    Matrix3&
    operator*= (const Matrix3& m);

  private:
    Vector3 m_right;
    Vector3 m_up;
    Vector3 m_back;
  };

  Matrix3
  operator+ (const Matrix3& m1, const Matrix3& m2);

  Matrix3
  operator- (const Matrix3& m1, const Matrix3& m2);

  Matrix3
  operator- (const Matrix3& m);

  Matrix3
  operator* (const Matrix3& m, float scalar);

  Matrix3
  operator* (float scalar, const Matrix3& m);

  Matrix3
  operator* (const Matrix3& m1, const Matrix3& m2);

  Vector3
  operator* (const Matrix3& m, const Vector3& v);

  std::ostream&
  operator<< (std::ostream& outStream, const Matrix3& m);
}

#endif
