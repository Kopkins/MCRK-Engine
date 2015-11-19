#include <cstdlib>
#include <cstdio>
#include <string>
#include <vector>
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Mesh.h"
#include "Texture.h"

#define BUFFER_OFFSET(byteOffset) (reinterpret_cast<void *> (byteOffset))

Mesh::Mesh () :
    m_vao (), m_transform (), m_tex (new Texture)
{
  glGenVertexArrays (1, &m_vao);
}

Mesh::~Mesh ()
{

}

void
Mesh::addGeometry (std::shared_ptr<VertexBuffer> newvertices)
{
  m_vertexBuffer = newvertices;
}

void
Mesh::prepareVao (const ShaderProgram shader)
{
  glBindVertexArray (m_vao);
  glBindBuffer (GL_ARRAY_BUFFER, m_vertexBuffer->getVbo ());

  int size = m_vertexBuffer->size () * sizeof(float);
  glBufferData (GL_ARRAY_BUFFER, size, m_vertexBuffer->data (), GL_STATIC_DRAW);

  GLint posAttrib = shader.getAttributeLocation ("vPosition");
  glEnableVertexAttribArray (posAttrib);
  glVertexAttribPointer (posAttrib, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
			 BUFFER_OFFSET(0));

  GLint colorAttrib = shader.getAttributeLocation ("vNormal");
  glEnableVertexAttribArray (colorAttrib);
  glVertexAttribPointer (colorAttrib, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
			 BUFFER_OFFSET(3 * sizeof(float)));

  GLint texAttrib = shader.getAttributeLocation ("vTexCoord");
  glEnableVertexAttribArray (texAttrib);
  glVertexAttribPointer (texAttrib, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
			 BUFFER_OFFSET(6 * sizeof(float)));
  glBindVertexArray (0);
}

void
Mesh::loadTexture (std::string file)
{
  m_tex->setData (file);
}

void
Mesh::draw (ShaderProgram shader)
{
  shader.enable ();
  glActiveTexture (GL_TEXTURE0);
  m_tex->bind ();
  glBindVertexArray (m_vao);
  glDrawArrays (GL_TRIANGLES, 0, m_vertexBuffer->size () / 6);
  glBindVertexArray (0);
  m_tex->unbind ();
  shader.disable ();
}

void
Mesh::moveRight (float distance)
{
  m_transform.moveRight (distance);
}

void
Mesh::moveUp (float distance)
{
  m_transform.moveUp (distance);
}

void
Mesh::moveBack (float distance)
{
  m_transform.moveBack (distance);
}

void
Mesh::moveLocal (float distance, const Vector3& localDirection)
{
  m_transform.moveLocal (distance, localDirection);
}

void
Mesh::moveWorld (float distance, const Vector3& worldDirection)
{
  m_transform.moveWorld (distance, worldDirection);
}

void
Mesh::yaw (float angleDegrees)
{
  m_transform.yaw (angleDegrees);
}

void
Mesh::pitch (float angleDegrees)
{
  m_transform.pitch (angleDegrees);
}

void
Mesh::roll (float angleDegrees)
{
  m_transform.roll (angleDegrees);
}

void
Mesh::rotateLocal (float angleDegrees, const Vector3& axis)
{
  m_transform.rotateLocal (angleDegrees, axis);
}

void
Mesh::alignWithWorldY ()
{
  m_transform.alignWithWorldY ();
}

void
Mesh::scaleLocal (float scale)
{
  m_transform.scaleLocal (scale);
}

void
Mesh::scaleLocal (float scaleX, float scaleY, float scaleZ)
{
  m_transform.scaleLocal (scaleX, scaleY, scaleZ);
}

void
Mesh::scaleWorld (float scale)
{
  m_transform.scaleWorld (scale);
}

void
Mesh::scaleWorld (float scaleX, float scaleY, float scaleZ)
{
  m_transform.scaleWorld (scaleX, scaleY, scaleZ);
}

void
Mesh::shearLocalXByYz (float shearY, float shearZ)
{
  m_transform.shearLocalXByYz (shearY, shearZ);
}

void
Mesh::shearLocalYByXz (float shearX, float shearZ)
{
  m_transform.shearLocalYByXz (shearX, shearZ);
}

void
Mesh::shearLocalZByXy (float shearX, float shearY)
{
  m_transform.shearLocalZByXy (shearX, shearY);
}

void
Mesh::getTransform (float array[16]) const
{
  m_transform.getTransform (array);
}

Vector3
Mesh::getPosition () const
{
  return m_transform.getPosition ();
}

void
Mesh::setPosition (float x, float y, float z)
{
  m_transform.setPosition (x, y, z);
}

/*
 void Mesh::clone(std::vector<Mesh*>& meshVector){
 meshVector.push_back(new Mesh);
 meshVector[meshVector.size()-1]->addGeometry(m_vertexBuffer);
 meshVector[meshVector.size()-1]->setPosition(meshVector[meshVector.size()-2]->getPosition().x-.4,meshVector[meshVector.size()-2]->getPosition().y+.4,meshVector[meshVector.size()-2]->getPosition().z);
 }
 */

Mesh*
Mesh::clone ()
{
  Mesh* ret = new Mesh;
  ret->m_tex = m_tex;
  ret->m_transform = m_transform;
  ret->m_vao = m_vao;
  ret->m_vertexBuffer = m_vertexBuffer;
  return ret;
}

const Material&
Mesh::getMaterial () const
{
  return m_material;
}

void
Mesh::setMaterial (const Material& material)
{
  m_material = material;
}

