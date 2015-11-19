#ifndef MATRIX4_H
#define MATRIX4_H

#include <iostream>

#include "Matrix3.h"
#include "Vector3.h"

namespace Math
{
  // Represent a 4x4 matrix using column-major storage.
  // Matrix is interpreted thus:
  // [ m_00 m_10 m_20 m_30 ] 
  // [ m_01 m_11 m_21 m_31 ] 
  // [ m_02 m_12 m_22 m_32 ] 
  // [ m_03 m_13 m_23 m_33 ] 
  class Matrix4
  {
  public:
    // Leave the matrix uninitialized
    Matrix4 ();

    // Initialize to identity regardless of the value of "makeIdentity"
    explicit
    Matrix4 (bool makeIdentity);

    // Assume "arrayOf16" uses column-major storage. 
    explicit
    Matrix4 (const float arrayOf16[16]);

    // Initialize upper 3x3 with "m", and other components to 0.
    // Put 1 in m_33.
    explicit
    Matrix4 (const Matrix3& m);

    // Do same as constructor above. 
    void
    setFromMatrix (const Matrix3& m);

    // Set to identity matrix. 
    void
    setToIdentity ();

    // Set to zero matrix. 
    void
    setToZero ();
    
    // Build a translation matrix.
    void
    setToTranslation (const Vector3& translation);

    // Build a uniform scale matrix.
    void
    setToScale (float scale);

    // Build a non-uniform scale matrix.
    void
    setToScale (float scaleX, float scaleY, float scaleZ);

    // Build rotation matrices. 
    void
    setToRotationX (float angleDegrees);

    void
    setToRotationY (float angleDegrees);

    void
    setToRotationZ (float angleDegrees);

    // Build an arbitrary axis rotation matrix. 
    void
    setFrontAngleAxis (float angleDegrees, const Vector3& unitAxis);

    // Assume "arrayOf16" uses column-major storage. 
    void
    getAsArray (float arrayOf16[16]) const;

    // Return a pointer to the first element. 
    const float*
    data () const;

    void
    transpose ();

    void
    negate ();

    void
    setToLookAt (const Vector3& eye, const Vector3& at, const Vector3& up);

    void
    setToPerspectiveProjection (double fovYDegrees, double aspectRatio,
				double nearPlaneZ, double farPlaneZ);

    void
    setToPerspectiveProjection (double left, double right, double bottom,
				double top, double nearPlaneZ,
				double farPlaneZ);

    void
    setToSymmetricPerspectiveProjection (double right, double top,
					 double nearPlaneZ,
					 double farPlaneZ);

    void
    setToOrthographicProjection (double left, double right, double bottom, double top, double nearPlaneZ, double farPlaneZ);

    // Operators
    Matrix4&
    operator+= (const Matrix4& m);

    Matrix4&
    operator-= (const Matrix4& m);

    Matrix4&
    operator*= (float s);

    Matrix4&
    operator*= (const Matrix4& m);

  private:
    union
    {
      float m_matrix[4][4];
      struct
      {
	float m_00, m_01, m_02, m_03;
	float m_10, m_11, m_12, m_13;
	float m_20, m_21, m_22, m_23;
	float m_30, m_31, m_32, m_33;
      };
    };
  };

  Matrix4
  operator+ (const Matrix4& m1, const Matrix4& m2);

  Matrix4
  operator- (const Matrix4& m1, const Matrix4& m2);

  // Unary negation
  Matrix4
  operator- (const Matrix4& m);

  Matrix4
  operator* (const Matrix4& m1, const Matrix4& m2);

  std::ostream&
  operator<< (std::ostream& outStream, const Matrix4& m);
}

#endif
