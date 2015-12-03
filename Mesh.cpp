#include "Mesh.h"

#define BUFFER_OFFSET(byteOffset) (reinterpret_cast<void *> (byteOffset))

Mesh::Mesh () :
    m_vao (), m_transform (), m_tex (new Texture), m_material (), m_shader (
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
  glBindVertexArray (0);
}

void
Mesh::loadTexture (std::string file)
{
  m_tex->setData (file);
}

void
Mesh::draw ()
{
  m_shader->enable ();
  glActiveTexture (GL_TEXTURE0);
  m_tex->bind ();
  glBindVertexArray (m_vao);
  glDrawArrays (GL_TRIANGLES, 0, m_vertexBuffer->size () / 6);
  glBindVertexArray (0);
  m_tex->unbind ();
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
  ret->m_tex = m_tex;
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
  std::cout<<m_material.shininess<<"\n";
  m_shader->disable();
}

