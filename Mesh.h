#ifndef Mesh_h
#define Mesh_h

#include <memory>
#include <vector>
#include "ShaderProgram.h"
#include "Transform.h"
#include "Texture.h"
#include "VertexBuffer.h"
#include "KeyBuffer.h"
#include "Material.cpp"

class Mesh{
  
public:

  Mesh();
  
  ~Mesh();

  Mesh(const std::vector<float>& vertices);

  void addGeometry (std::shared_ptr<VertexBuffer> newm_vertices);
  
  void outputGeometry();

  void prepareVao();
  
  void loadTexture(std::string file);

  void draw();

  // Key: "1"
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

  // Key: "2"
  void
  pitch (float angleDegrees);

  // Key: "3"
  void
  yaw (float angleDegrees);

  // Key: "4"
  void
  roll (float angleDegrees);

  void
  rotateLocal (float angleDegrees, const Vector3& axis);

  // Key: "5"
  void
  alignWithWorldY ();

  // Key: "6"
  void
  scaleLocal (float scale);

  void
  scaleLocal (float scaleX, float scaleY, float scaleZ);

  // Key: "7"
  void
  scaleWorld (float scale);

  void
  scaleWorld (float scaleX, float scaleY, float scaleZ);

  // Key: "8"
  void
  shearLocalXByYz (float shearY, float shearZ);

  // Key: "9"
  void
  shearLocalYByXz (float shearX, float shearZ);

  // Key: "0"
  void
  shearLocalZByXy (float shearX, float shearY);

  void
  getTransform(float array[16])const;

  Vector3
  getPosition()const;

  void setPosition(float x, float y, float z);

  void
  createModelViewMatrix (float array[16]);

  Mesh* clone();

  const Material&
  getMaterial () const;

  void
  setMaterial (const Material& material);

  void
  setShaderPointer(ShaderProgram* shader);

  void
  activateMaterial ();

  void
  yawPitchRoll(Vector3& v);

  void
  move (Vector3& v);

  void
  update (KeyBuffer keyBuffer, const float MOVEMENT_DELTA);

private:

  GLuint m_vao;
  std::shared_ptr<VertexBuffer> m_vertexBuffer;
  Math::Transform m_transform;
  std::shared_ptr<Texture> m_tex;
  Material m_material;
  ShaderProgram* m_shader;

};

#endif
