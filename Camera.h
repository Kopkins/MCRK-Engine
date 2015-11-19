#ifndef Camera_h
#define Camera_h

#include <cstdlib>
#include <glm/mat4x4.hpp>
#include <bitset>
#include "ShaderProgram.h"
#include "Vector3.h"
#include "Matrix3.h"
#include "Transform.h"
#include "Matrix4.h"

class Camera
{

  Math::Transform m_camera;
  float m_verticalFov, m_aspectRatio, m_nearZ, m_farZ;
  Math::Matrix4 m_projection;
  bool m_isOrtho;

  Vector3
  getRight (); //LOL Ninjas in Pyjamas

public:

  Camera ();

  void
  moveRight (float distance);

  void
  moveUp (float distance);

  void
  moveBack (float distance);

  void
  createModelViewMatrix (ShaderProgram& shaderProg, float array[16]);

  void
  createProjectionMatrix (ShaderProgram& shaderProg);

  void
  createProjectionMatrix (ShaderProgram& shaderProg,bool orthographic);

  Vector3
  getEye ();

  Vector3
  getUp ();

  Vector3
  getBack ();

  float
  getVerticalFov ();

  float
  getAspectRatio ();

  float
  getNearZ ();

  float
  getFarZ ();

  Math::Matrix4
  getProjectionMatrix();

  bool
  isOrtho();

  void
  setEye (float x, float y, float z);

  void
  setUp (float x, float y, float z);

  void
  setBack (float x, float y, float z);

  void
  setVerticalFov (float fov);

  void
  setAspectRatio (float aspectRatio);

  void
  setNearZ (float nearZ);

  void
  setFarZ (float farZ);

  void
  yaw (float angleDegrees);

  void
  pitch (float angleDegrees);

  void
  roll (float angleDegrees);

  void
  zoom(float deltaFov);

  void
  reset ();

};

#endif
