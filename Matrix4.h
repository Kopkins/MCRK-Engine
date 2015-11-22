#ifndef MATRIX4_H
#define MATRIX4_H

#include "Matrix3.h"
#include "Vector3.h"

namespace Math
{
  class Matrix4
  {
  public:
    Matrix4 ();

    explicit
    Matrix4 (bool makeIdentity);

    explicit
    Matrix4 (const float arrayOf16[16]);

    explicit
    Matrix4 (const Matrix3& m);

    void
    setFromMatrix (const Matrix3& m);

    void
    setToIdentity ();

    void
    setToZero ();
    
    void
    setToTranslation (const Vector3& translation);

    void
    setToScale (float scale);

    void
    setToScale (float scaleX, float scaleY, float scaleZ);

    void
    setToRotationX (float angleDegrees);

    void
    setToRotationY (float angleDegrees);

    void
    setToRotationZ (float angleDegrees);

    void
    setFrontAngleAxis (float angleDegrees, const Vector3& unitAxis);

    void
    getAsArray (float arrayOf16[16]) const;

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
