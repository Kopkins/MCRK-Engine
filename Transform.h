#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "Matrix3.h"
#include "Vector3.h"

namespace Math
{
  class Transform
  {
  public:
    Transform ();

    Transform (float array[16]);

    Transform (Matrix3& rotScale, Vector3& position);

    ~Transform ();

    void
    orthonormalize ();

    void
    reset ();

    Transform combine(Transform& t);

    void
    getTransform (float array[16]) const;

    Vector3
    getPosition () const;

    void
    setPosition (const Vector3& position);

    void
    setPosition (float positionX, float positionY, float positionZ);

    Vector3
    getRight () const;

    void
    setRight (const Vector3& right);

    Vector3
    getUp () const;

    void
    setUp (const Vector3& up);

    Vector3
    getBack () const;

    void
    setBack (const Vector3& back);

    void
    moveRight (float distance);

    void
    moveUp (float distance);

    void
    moveBack (float distance);

    void
    moveLocal (float distance, const Vector3& localDirection);

    void
    moveWorld (float distance, const Vector3& worldDirection);

    Matrix3
    getOrientation () const;

    void
    setOrientation (const Matrix3& orientation);

    void
    setOrientation (const Vector3& right, const Vector3& up,
		    const Vector3& backward);

    void
    pitch (float angleDegrees);

    void
    yaw (float angleDegrees);

    void
    roll (float angleDegrees);

    void
    rotateLocal (float angleDegrees, const Vector3& axis);

    void
    alignWithWorldY ();

    void
    rotateWorld (float angleDegrees, const Vector3& axis);

    void
    scaleLocal (float scale);

    void
    scaleLocal (float scaleX, float scaleY, float scaleZ);

    void
    scaleWorld (float scale);

    void
    scaleWorld (float scaleX, float scaleY, float scaleZ);

    void
    shearLocalXByYz (float shearY, float shearZ);

    void
    shearLocalYByXz (float shearX, float shearZ);

    void
    shearLocalZByXy (float shearX, float shearY);

  private:
    Matrix3 m_rotScale;
    Vector3 m_position;
  };
}

#endif
