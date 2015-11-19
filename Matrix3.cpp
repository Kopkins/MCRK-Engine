#include <cstdlib>
#include <cstdio>
#include <string>
#include <vector>
#include <iostream>
#include <cmath>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Matrix3.h"

namespace Math
{
  Matrix3::Matrix3 () :
      m_right (1, 0, 0), m_up (0, 1, 0), m_back (0, 0, 1)
  {
  }

  Matrix3::Matrix3 (float rx, float ry, float rz, float ux, float uy, float uz,
		    float bx, float by, float bz) :
      m_right (rx, ry, rz), m_up (ux, uy, uz), m_back (bx, by, bz)
  {
  }

  Matrix3::Matrix3 (const Vector3& right, const Vector3& up,
		    const Vector3& back) :
      m_right (right), m_up (up), m_back (back)
  {
  }

  Matrix3::Matrix3 (const Vector3& right, const Vector3& up,
		    bool makeOrthonormal) :
      m_right (right), m_up (up), m_back (right.cross (up))
  {
    if (makeOrthonormal == true)
      orthonormalize ();
  }

  void
  Matrix3::setToIdentity ()
  {
    m_right.set (1, 0, 0);
    m_up.set (0, 1, 0);
    m_back.set (0, 0, 1);
  }

  void
  Matrix3::setToZero ()
  {
    m_right.set (0, 0, 0);
    m_up.set (0, 0, 0);
    m_back.set (0, 0, 0);
  }

  const float*
  Matrix3::data () const
  {
    return &m_right.x;
  }

  void
  Matrix3::setRight (const Vector3& right)
  {
    m_right.x = right.x;
    m_right.y = right.y;
    m_right.z = right.z;
  }

  Vector3
  Matrix3::getRight () const
  {
    return m_right;
  }

  void
  Matrix3::setUp (const Vector3& up)
  {
    m_up.x = up.x;
    m_up.y = up.y;
    m_up.z = up.z;
  }

  Vector3
  Matrix3::getUp () const
  {
    return m_up;
  }

  void
  Matrix3::setBack (const Vector3& back)
  {
    m_back.x = back.x;
    m_back.y = back.y;
    m_back.z = back.z;
  }

  Vector3
  Matrix3::getBack () const
  {
    return m_back;
  }

  void
  Matrix3::setForward (const Vector3& forward)
  {
    m_back.x = -1 * forward.x;
    m_back.y = -1 * forward.y;
    m_back.z = -1 * forward.z;
  }

  Vector3
  Matrix3::getForward () const
  {
    Vector3 temp = m_back * -1.0f;
    return temp;
  }

  void
  Matrix3::invertRotation ()
  {
    transpose ();
  }

  void
  Matrix3::invert ()
  {
    Matrix3 thing ((m_up.y * m_back.z) - (m_back.y * m_up.z),
		   (m_back.y * m_right.z) - (m_right.y * m_back.z),
		   (m_right.y * m_up.z) - (m_up.y - m_right.z),
		   (m_back.x * m_up.z) - (m_up.x - m_back.z),
		   (m_right.x * m_back.z) - (m_back.x * m_right.z),
		   (m_up.x * m_right.z) - (m_right.x * m_up.z),
		   (m_up.x * m_back.y) * (m_back.x * m_up.y),
		   (m_back.x * m_right.y) - (m_right.x * m_back.y),
		   (m_right.x * m_right.z) - (m_up.x * m_right.y));
    thing *= 1 / determinant ();
  }

  float
  Matrix3::determinant () const
  {
    return m_right.x * ((m_up.y * m_back.z) - (m_up.z * m_back.y))
	- m_right.y * ((m_up.x * m_back.z) - (m_up.z * m_back.x))
	+ m_right.z * ((m_up.x * m_back.y) - (m_up.y * m_back.x));
  }

  void
  Matrix3::transpose ()
  {
    float temp = m_right.z;
    m_right.z = m_back.x;
    m_back.x = temp;
    temp = m_right.y;
    m_right.y = m_up.x;
    m_up.x = temp;
    temp = m_up.z;
    m_up.z = m_back.y;
    m_back.y = temp;
  }

  void
  Matrix3::orthonormalize ()
  {
    m_back.normalize ();
    m_up = m_back.cross (m_right);
    m_up.normalize ();
    m_right = m_up.cross (m_back);
  }

  void
  Matrix3::setToScale (float scale)
  {
    m_right.set (scale, 0, 0);
    m_up.set (0, scale, 0);
    m_back.set (0, 0, scale);
  }

  void
  Matrix3::setToScale (float scaleX, float scaleY, float scaleZ)
  {
    m_right.set (scaleX, 0, 0);
    m_up.set (0, scaleY, 0);
    m_back.set (0, 0, scaleZ);
  }

  void
  Matrix3::setToShearXByYz (float shearY, float shearZ)
  {
    m_right.set (1, 0, 0);
    m_up.set (shearY, 1, 0);
    m_back.set (shearZ, 0, 1);
  }

  void
  Matrix3::setToShearYByXz (float shearX, float shearZ)
  {
    m_right.set (1, shearX, 0);
    m_up.set (0, 1, 0);
    m_back.set (0, shearZ, 1);
  }

  void
  Matrix3::setToShearZByXy (float shearX, float shearY)
  {
    m_right.set (1, 0, shearX);
    m_up.set (0, 1, shearY);
    m_back.set (0, 0, 1);
  }

  void
  Matrix3::setToRotationX (float angleDegrees)
  {
    m_right.set (1, 0, 0);
    m_up.set (0, cos (angleDegrees), sin (angleDegrees));
    m_back.set (0, -1 * sin (angleDegrees), cos (angleDegrees));
  }

  void
  Matrix3::setToRotationY (float angleDegrees)
  {
    m_right.set (cos (angleDegrees), 0, -1 * sin (angleDegrees));
    m_up.set (0, 1, 0);
    m_back.set (sin (angleDegrees), 0, cos (angleDegrees));
  }

  void
  Matrix3::setToRotationZ (float angleDegrees)
  {
    m_right.set (cos (angleDegrees), sin (angleDegrees), 0);
    m_up.set (-1 * sin (angleDegrees), cos (angleDegrees), 0);
    m_back.set (0, 0, 1);
  }

  void
  Matrix3::setFromAngleAxis (float angleDegrees, const Vector3& axis)
  {
    m_right.set (
	cos (angleDegrees) + axis.x * axis.x * (1 - cos (angleDegrees)),
	axis.y * axis.x * (1 - cos (angleDegrees))+ axis.z * sin (angleDegrees),
	axis.z * axis.x * (1 - cos (angleDegrees))- axis.y * sin (angleDegrees));
    m_up.set (
	axis.y * axis.x * (1 - cos (angleDegrees))- axis.z * sin (angleDegrees),
	cos (angleDegrees) + axis.y * axis.y * (1 - cos (angleDegrees)),
	axis.z * axis.y * (1 - cos (angleDegrees))+ axis.x * sin (angleDegrees));
    m_back.set (
	axis.x * axis.z * (1 - cos (angleDegrees))+ axis.y * sin (angleDegrees),
	axis.y * axis.z * (1 - cos (angleDegrees))- axis.x * sin (angleDegrees),
	cos (angleDegrees) + axis.z * axis.z * (1 - cos (angleDegrees)));
  }

  void
  Matrix3::negate ()
  {
    m_right.negate ();
    m_up.negate ();
    m_right.negate ();
  }

  Vector3
  Matrix3::transform (const Vector3& point) const
  {
    return *this * point;
  }

  Matrix3&
  Matrix3::operator += (const Matrix3& m)
  {
    this->m_right += m.getRight ();
    this->m_up += m.getUp ();
    this->m_back += m.getBack ();
    return *this;
  }

  Matrix3&
  Matrix3::operator -= (const Matrix3& m)
  {
    this->m_right -= m.getRight ();
    this->m_up -= m.getUp ();
    this->m_back -= m.getBack ();
    return *this;
  }

  Matrix3&
  Matrix3::operator *= (float scalar)
  {
    this->m_right *= scalar;
    this->m_up *= scalar;
    this->m_back *= scalar;
    return *this;
  }

  Matrix3&
  Matrix3::operator *= (const Matrix3& m)
  {
    *this = *this * m;
    return *this;
  }

  Matrix3
  operator+ (const Matrix3& m1, const Matrix3& m2)
  {
    return Matrix3 (m1.getRight () + m2.getRight (), m2.getUp () + m2.getUp (),
		    m1.getBack () + m2.getBack ());
  }

  Matrix3
  operator- (const Matrix3& m1, const Matrix3& m2)
  {
    return Matrix3 (m1.getRight () - m2.getRight (), m2.getUp () - m2.getUp (),
		    m1.getBack () - m2.getBack ());
  }

// Unary negation
  Matrix3
  operator- (const Matrix3& m)
  {
    return Matrix3 (m.getRight () * -1, m.getUp () * -1, m.getBack () * -1);
  }

  Matrix3
  operator* (const Matrix3& m, float scalar)
  {
    return Matrix3 (m.getRight () * scalar, m.getUp () * scalar,m.getBack () * scalar);
  }

  Matrix3
  operator* (float scalar, const Matrix3& m)
  {
    return Matrix3 (m.getRight () * scalar, m.getUp () * scalar, m.getBack () * scalar);
  }

  Matrix3
  operator* (const Matrix3& m1, const Matrix3& m2)
  {
    return Matrix3 (
	Vector3 (
	    m1.getRight () * m2.getRight ().x + m1.getUp () * m2.getRight ().y
		+ m1.getBack () * m2.getRight ().z),
	Vector3 (
	    m1.getRight () * m2.getUp ().x + m1.getUp () * m2.getUp ().y
		+ m1.getBack () * m2.getUp ().z),
	Vector3 (
	    m1.getRight () * m2.getBack ().x + m1.getUp () * m2.getBack ().y
		+ m1.getBack () * m2.getBack ().z));
  }

  Vector3
  operator* (const Matrix3& m, const Vector3& v)
  {
    return Vector3 (m.getRight () * v.x + m.getUp () * v.y + m.getBack () * v.z);
  }

}

