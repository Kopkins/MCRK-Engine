#include "ShaderProgram.h"
#include "Camera.h"
#include "Vector3.h"

using namespace Math;

Camera::Camera () :
    m_camera (), m_verticalFov (), m_aspectRatio (), m_nearZ (), m_farZ (), m_projection(), m_isOrtho()
{

}

void
Camera::moveRight (float distance)
{
  m_camera.setPosition (
      m_camera.getPosition () + m_camera.getRight () * distance);
}

void
Camera::moveUp (float distance)
{
  m_camera.setPosition (m_camera.getPosition () + m_camera.getUp () * distance);
}

void
Camera::moveBack (float distance)
{
  m_camera.setPosition (
      m_camera.getPosition () + m_camera.getBack () * distance);
}

void
Camera::createProjectionMatrix (ShaderProgram& shaderProg)
{
  m_projection.setToPerspectiveProjection(m_verticalFov,m_aspectRatio,m_nearZ,m_farZ);
  float projection[16];
  m_projection.getAsArray(projection);
  shaderProg.enable();
  GLint projectionLoc=shaderProg.getUniformLocation ("vProjection");
  shaderProg.setUniformMatrix4fv(projectionLoc,1,GL_FALSE,projection);
  m_isOrtho=false;
}

void
Camera::createProjectionMatrix (ShaderProgram& shaderProg, bool orthographic)
{
  m_projection.setToOrthographicProjection(-2,2,-1.5,1.5,m_nearZ,m_farZ);
  float projection[16];
  m_projection.getAsArray(projection);
  shaderProg.enable();
  GLint projectionLoc=shaderProg.getUniformLocation ("vProjection");
  shaderProg.setUniformMatrix4fv(projectionLoc,1,GL_FALSE,projection);
  m_isOrtho=true;
}

Vector3
Camera::getEye ()
{
  return m_camera.getPosition ();
}

Vector3
Camera::getUp ()
{
  return m_camera.getUp ();
}

Vector3
Camera::getBack ()
{
  return m_camera.getBack ();
}

Vector3
Camera::getRight ()
{
  return m_camera.getRight ();
}

float
Camera::getVerticalFov ()
{
  return m_verticalFov;
}

float
Camera::getAspectRatio ()
{
  return m_aspectRatio;
}

float
Camera::getNearZ ()
{
  return m_nearZ;
}

float
Camera::getFarZ ()
{
  return m_farZ;
}

bool
Camera::isOrtho(){
  return m_isOrtho;
}

void
Camera::setEye (float x, float y, float z)
{
  m_camera.setPosition (x, y, z);
}

void
Camera::setUp (float x, float y, float z)
{
  m_camera.setUp (Vector3 (x, y, z));
}

void
Camera::setBack (float x, float y, float z)
{
  m_camera.setBack (Vector3 (x, y, z));
}

void
Camera::setVerticalFov (float fov)
{
  m_verticalFov = fov;
}

void
Camera::setAspectRatio (float aspectRatio)
{
  m_aspectRatio = aspectRatio;
}

void
Camera::setNearZ (float nearZ)
{
  m_nearZ = nearZ;
}

void
Camera::setFarZ (float farZ)
{
  m_farZ = farZ;
}

void
Camera::yaw (float angleDegrees)
{
  m_camera.yaw (angleDegrees);
}

void
Camera::pitch (float angleDegrees)
{
  m_camera.pitch (angleDegrees);
}

void
Camera::roll (float angleDegrees)
{
  m_camera.roll (angleDegrees);
}

void
Camera::zoom(float deltaFov){
  setVerticalFov(m_verticalFov+deltaFov);
}

void
Camera::reset ()
{
  m_camera.reset();
}

void
Camera::getTransform (float array[16]) const
{
  m_camera.getTransform (array);
}


