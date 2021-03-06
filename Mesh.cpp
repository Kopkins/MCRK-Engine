#include <GL/glew.h>
#include "Mesh.h"

#define BUFFER_OFFSET(byteOffset) (reinterpret_cast<void *> (byteOffset))

Mesh::Mesh () :
    m_vao (), m_vertexBuffer(NULL), m_indexBuffer(NULL), m_transform (), m_dif (new Texture), m_spec(new Texture),
    m_material (), m_shader (
	nullptr)
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
Mesh::addIndices (std::shared_ptr<IndexBuffer> indices)
{
  m_indexBuffer = indices;
}

void
Mesh::prepareVao ()
{
  glBindVertexArray (m_vao);
  glBindBuffer (GL_ARRAY_BUFFER, m_vertexBuffer->getVbo ());

  int size = m_vertexBuffer->size () * sizeof(float);
  glBufferData (GL_ARRAY_BUFFER, size, m_vertexBuffer->data (), GL_STATIC_DRAW);

  GLint posAttrib = m_shader->getAttributeLocation ("vPosition");
  glEnableVertexAttribArray (posAttrib);
  glVertexAttribPointer (posAttrib, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
			 BUFFER_OFFSET(0));

  GLint colorAttrib = m_shader->getAttributeLocation ("vNormal");
  glEnableVertexAttribArray (colorAttrib);
  glVertexAttribPointer (colorAttrib, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
			 BUFFER_OFFSET(3 * sizeof(float)));

  GLint texAttrib = m_shader->getAttributeLocation ("vTexCoord");
  glEnableVertexAttribArray (texAttrib);
  glVertexAttribPointer (texAttrib, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
			 BUFFER_OFFSET(6 * sizeof(float)));

  size = m_indexBuffer->size () * sizeof(unsigned int);
  GLuint elementBuf;
  glGenBuffers (1, &elementBuf);
  glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, elementBuf);
  glBufferData (GL_ELEMENT_ARRAY_BUFFER, size, m_indexBuffer->data (), GL_STATIC_DRAW);

  glBindVertexArray (0);

}

void
Mesh::loadTexture (int type, std::string file)
{
  if (type==0)
    m_dif->setData (file);
  else
    m_spec->setData(file);
}

void
Mesh::draw ()
{
  m_shader->enable ();
  glActiveTexture (GL_TEXTURE0);
  m_dif->bind ();
  glActiveTexture (GL_TEXTURE1);
  m_spec->bind();
  glBindVertexArray (m_vao);
  glDrawElements (GL_TRIANGLES, m_indexBuffer->size (), GL_UNSIGNED_INT, BUFFER_OFFSET(0));
  glBindVertexArray (0);
  m_spec->unbind ();
  m_dif->unbind ();
  m_shader->disable ();
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

void
Mesh::createModelViewMatrix (float array[16])
{
  Math::Transform camTransform (array);
  Math::Matrix3 mat = camTransform.getOrientation ();
  mat.transpose ();
  Vector3 vec = camTransform.getPosition ();
  vec.negate ();
  vec = mat * vec;
  Math::Transform view (mat, vec);

  float modelView[16];
  Math::Transform combined = view.combine (m_transform);
  combined.getTransform (modelView);

  m_shader->enable ();
  GLint modelViewLoc = m_shader->getUniformLocation ("vModelView");
  m_shader->setUniformMatrix4fv (modelViewLoc, 1, GL_FALSE, modelView);
}

Mesh*
Mesh::clone ()
{
  Mesh* ret = new Mesh;
  ret->m_dif = m_dif;
  ret->m_spec = m_spec;
  ret->m_transform = m_transform;
  ret->m_vao = m_vao;
  ret->m_vertexBuffer = m_vertexBuffer;
  ret->m_material = m_material;
  ret->m_shader = m_shader;
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

void
Mesh::setShaderPointer (ShaderProgram* shader)
{
  m_shader = shader;
}

void
Mesh::activateMaterial ()
{
  m_shader->enable();
  GLint materialLoc = m_shader->getUniformLocation ("material.ambientRefl");
  m_shader->setUniform3fv (materialLoc, 1, &m_material.ambientRefl.x);
  materialLoc = m_shader->getUniformLocation ("material.diffuseRefl");
  m_shader->setUniform3fv (materialLoc, 1, &m_material.diffuseRefl.x);
  materialLoc = m_shader->getUniformLocation ("material.specularRefl");
  m_shader->setUniform3fv (materialLoc, 1	, &m_material.specularRefl.x);
  materialLoc = m_shader->getUniformLocation ("material.shininess");
  m_shader->setUniform1f (materialLoc, m_material.shininess);
  m_shader->disable();
}

void
Mesh::yawPitchRoll(Vector3& v)
{
  yaw(v.x);
  pitch(v.y);
  roll(v.z);
}

void
Mesh::move (Vector3& v)
{
  moveRight(v.x);
  moveUp(v.y);
  moveBack(v.z);
}

void
Mesh::update (KeyBuffer keyBuffer, const float MOVEMENT_DELTA)
{

  Vector3 dxyz(0), dypr(0);
  auto buf = keyBuffer.getBuffer ();
  if (buf[GLFW_KEY_5])
    {
      alignWithWorldY();
     return;
    }
  if (buf[GLFW_KEY_6])
    {
      scaleLocal (1.01f);
      return;
    }

  if (buf[GLFW_KEY_7])
    {
      scaleWorld (0.99f);
      return;
    }
  dxyz.x = buf[GLFW_KEY_RIGHT] - buf[GLFW_KEY_LEFT];
  dxyz.y = buf[GLFW_KEY_PAGE_UP] - buf[GLFW_KEY_PAGE_DOWN];
  dxyz.z = buf[GLFW_KEY_DOWN] - buf[GLFW_KEY_UP];
  dypr.y = buf[GLFW_KEY_2];
  dypr.x = -3;
  dypr.z = buf[GLFW_KEY_4];
  dxyz *= MOVEMENT_DELTA;
  dypr *= MOVEMENT_DELTA / 10;
  move (dxyz);
  yawPitchRoll (dypr);
  float sxyz, syxz, szxy;
  sxyz = buf[GLFW_KEY_8] * MOVEMENT_DELTA / 10;
  syxz = buf[GLFW_KEY_9] * MOVEMENT_DELTA / 10;
  szxy = buf[GLFW_KEY_0] * MOVEMENT_DELTA / 10;
  shearLocalXByYz(sxyz, sxyz);
  shearLocalYByXz(syxz, syxz);
  shearLocalZByXy(szxy, szxy);
}

