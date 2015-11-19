#include <cstdlib>
#include <cstdio>
#include <string>
#include <vector>
#include <iostream>
#include <cmath>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Transform.h"

namespace Math
{
  Transform::Transform () :
      m_rotScale (), m_position ()
  {

  }

  Transform::Transform (float array[16]) :
      m_rotScale (Vector3 (array[0], array[1], array[2]),
		  Vector3 (array[4], array[5], array[6]),
		  Vector3 (array[8], array[9], array[10])), m_position (
	  array[12], array[13], array[14])
  {

  }

  Transform::Transform (Matrix3& rotScale, Vector3& position) :
      m_rotScale (rotScale), m_position (position)
  {

  }

  Transform::~Transform ()
  {

  }

  void
  Transform::orthonormalize ()
  {
    m_rotScale.orthonormalize ();
  }

  void
  Transform::reset ()
  {
    m_rotScale.setToIdentity ();
  }

  Transform
  Transform::combine (Transform& t)
  {
    Matrix3 mat = m_rotScale * t.getOrientation ();
    Vector3 vec = (m_rotScale * t.getPosition ()) + m_position;
    return Transform (mat, vec);
  }

  void
  Transform::getTransform (float array[16]) const
  {
    array[0] = m_rotScale.getRight ().x;
    array[1] = m_rotScale.getRight ().y;
    array[2] = m_rotScale.getRight ().z;
    array[3] = 0.0f;
    array[4] = m_rotScale.getUp ().x;
    array[5] = m_rotScale.getUp ().y;
    array[6] = m_rotScale.getUp ().z;
    array[7] = 0.0f;
    array[8] = m_rotScale.getBack ().x;
    array[9] = m_rotScale.getBack ().y;
    array[10] = m_rotScale.getBack ().z;
    array[11] = 0.0f;
    array[12] = m_position.x;
    array[13] = m_position.y;
    array[14] = m_position.z;
    array[15] = 1.0f;
  }

  Vector3
  Transform::getPosition () const
  {
    return m_position;
  }

  void
  Transform::setPosition (const Vector3& position)
  {
    m_position = position;
  }

  void
  Transform::setPosition (float positionX, float positionY, float positionZ)
  {
    m_position.x = positionX;
    m_position.y = positionY;
    m_position.z = positionZ;
  }

  Vector3
  Transform::getRight () const
  {
    return m_rotScale.getRight ();
  }

  void
  Transform::setRight (const Vector3& right)
  {
    m_rotScale.setRight (right);
  }

  Vector3
  Transform::getUp () const
  {
    return m_rotScale.getUp ();
  }

  void
  Transform::setUp (const Vector3& up)
  {
    m_rotScale.setUp (up);
  }

  Vector3
  Transform::getBack () const
  {
    return m_rotScale.getBack ();
  }

  void
  Transform::setBack (const Vector3& back)
  {
    m_rotScale.setBack (back);
  }

  void
  Transform::moveRight (float distance)
  {
    m_position += getRight () * distance;
  }

  void
  Transform::moveUp (float distance)
  {
    m_position += getUp () * distance;
  }

  void
  Transform::moveBack (float distance)
  {
    m_position += getBack () * distance;
  }

  void
  Transform::moveLocal (float distance, const Vector3& localDirection)
  {
    Matrix3 mat = m_rotScale;
    mat.transpose ();
    Vector3 vec = mat * localDirection - m_position;
    m_position += distance * vec;
  }

  void
  Transform::moveWorld (float distance, const Vector3& worldDirection)
  {
    m_position += worldDirection * distance;
  }

  Matrix3
  Transform::getOrientation () const
  {
    return m_rotScale;
  }

  void
  Transform::setOrientation (const Matrix3& orientation)
  {
    m_rotScale = orientation;
  }

  void
  Transform::setOrientation (const Vector3& right, const Vector3& up,
			     const Vector3& backward)
  {
    m_rotScale.setRight (right);
    m_rotScale.setUp (up);
    m_rotScale.setBack (backward);
  }

  void
  Transform::yaw (float angleDegrees)
  {
    Matrix3 temp;
    temp.setToRotationY (angleDegrees);
    m_rotScale *= temp;
  }

  void
  Transform::pitch (float angleDegrees)
  {
    Matrix3 temp;
    temp.setToRotationX (angleDegrees);
    m_rotScale *= temp;
  }

  void
  Transform::roll (float angleDegrees)
  {
    Matrix3 temp;
    temp.setToRotationZ (angleDegrees);
    m_rotScale *= temp;
  }

  void
  Transform::rotateLocal (float angleDegrees, const Vector3& axis)
  {
    Matrix3 temp;
    temp.setFromAngleAxis (angleDegrees, axis);
    m_rotScale = m_rotScale * temp;
  }

  void
  Transform::alignWithWorldY ()
  {
    m_rotScale.setUp (Vector3 (0, 1, 0));
    m_rotScale.setBack (m_rotScale.getRight ().cross (m_rotScale.getUp ()));
    Vector3 vec = m_rotScale.getBack ();
    vec.normalize ();
    m_rotScale.setBack (vec);
    m_rotScale.setRight (m_rotScale.getUp ().cross (m_rotScale.getBack ()));
  }

  void
  Transform::rotateWorld (float angleDegrees, const Vector3& axis)
  {
    Matrix3 temp;
    temp.setFromAngleAxis (angleDegrees, axis);
    m_rotScale = temp * m_rotScale;
  }

  void
  Transform::scaleLocal (float scale)
  {
    Matrix3 temp;
    temp.setToScale (scale);
    m_rotScale = m_rotScale * temp;
  }

  void
  Transform::scaleLocal (float scaleX, float scaleY, float scaleZ)
  {
    Matrix3 temp;
    temp.setToScale (scaleX, scaleY, scaleZ);
    m_rotScale = m_rotScale * temp;
  }

  void
  Transform::scaleWorld (float scale)
  {
    Matrix3 temp;
    temp.setToScale (scale);
    m_rotScale = temp * m_rotScale;
    m_position = temp * m_position;
  }

  void
  Transform::scaleWorld (float scaleX, float scaleY, float scaleZ)
  {
    Matrix3 temp;
    temp.setToScale (scaleX, scaleY, scaleZ);
    m_rotScale = temp * m_rotScale;
    m_position = temp * m_position;
  }

  void
  Transform::shearLocalXByYz (float shearY, float shearZ)
  {
    Matrix3 temp;
    temp.setToShearXByYz (shearY, shearZ);
    m_rotScale = m_rotScale * temp;
  }

  void
  Transform::shearLocalYByXz (float shearX, float shearZ)
  {
    Matrix3 temp;
    temp.setToShearYByXz (shearX, shearZ);
    m_rotScale = m_rotScale * temp;
  }

  void
  Transform::shearLocalZByXy (float shearX, float shearY)
  {
    Matrix3 temp;
    temp.setToShearZByXy (shearX, shearY);
    m_rotScale = m_rotScale * temp;
  }

}
