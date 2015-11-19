// Matrix3 class for manipulating 3x3 matrices. 
// by Gary M. Zoppetti, Ph.D.
// Department of Computer Science
// Millersville University of Pennsylvania

#ifndef MATRIX3_H
#define MATRIX3_H

#include <iostream>

#include "Vector3.h"

namespace Math
{
  // Basis vectors are stored in Vector3's and form
  //   the columns of a 3x3 matrix. 
  // Matrix is interpreted thus:
  // [ rx ux bx ]
  // [ ry uy by ]
  // [ rz uz bz ]
  // Operations are consistent with column vectors (v' = M * v).
  class Matrix3
  {
  public:
    // Initialize to identity. 
    Matrix3 ();

    // Initial right vector to (rz, ry, rz), etc.
    Matrix3 (float rx, float ry, float rz, float ux, float uy, float uz,
	     float bx, float by, float bz);

    // Initialize corresponding basis vectors
    Matrix3 (const Vector3& right, const Vector3& up, const Vector3& back);

    // Initialize right, up, and back. Compute back.
    // If "makeOrthonormal" is true, ensure each vector is of
    //   unit length and all 3 vectors are mutually perpendicular.
    Matrix3 (const Vector3& right, const Vector3& up, bool makeOrthonormal =
		 false);

    // Set to identity matrix.
    void
    setToIdentity ();

    // Set all elements to 0.
    void
    setToZero ();

    // Return a pointer to the first element
    const float*
    data () const;

    // Set the right vector.
    void
    setRight (const Vector3& right);

    // Get the right vector.
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

    // Set the back vector to the negation of "forward".
    void
    setForward (const Vector3& forward);

    Vector3
    getForward () const;

    // Invert a rotation matrix.
    void
    invertRotation ();

    // Invert a general matrix.
    void
    invert ();

    float
    determinant () const;

    void
    transpose ();

    // Ensure all axes are of unit length and mutually perpendicular.
    void
    orthonormalize ();

    // Set to a uniform scale matrix.
    void
    setToScale (float scale);

    // Set to a non-uniform scale matrix.
    void
    setToScale (float scaleX, float scaleY, float scaleZ);

    // Set to a shear matrix that shears x by y and z.
    void
    setToShearXByYz (float shearY, float shearZ);

    // Set to a shear matrix that shears y by x and z.
    void
    setToShearYByXz (float shearX, float shearZ);

    // Set to a shear matrix that shears z by x and y.
    void
    setToShearZByXy (float shearX, float shearY);

    // Set to an x-axis rotation matrix.
    void
    setToRotationX (float angleDegrees);

    void
    setToRotationY (float angleDegrees);

    void
    setToRotationZ (float angleDegrees);

    // Set to a general rotation matrix about "axis". 
    void
    setFromAngleAxis (float angleDegrees, const Vector3& axis);

    // Negate all entries.
    void
    negate ();

    // Compute *this * point.
    Vector3
    transform (const Vector3& point) const;

    Matrix3&
    operator+= (const Matrix3& m);

    Matrix3&
    operator-= (const Matrix3& m);

    Matrix3&
    operator*= (float scalar);

    // Matrix multiply.
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

  // Unary negation
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

  // Print "m" in a neat, tabular format.
  std::ostream&
  operator<< (std::ostream& outStream, const Matrix3& m);
}

#endif
