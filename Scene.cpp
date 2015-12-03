#include "Scene.h"

Scene::Scene() :
		m_names(), m_active(), m_meshs(), m_size(0) {
}

void
Scene::setActive(std::string name) {
  if (m_names.count(name))
    m_active = name;
}

Mesh*
Scene::getMesh(std::string name) {
  return m_meshs.at(name);
}

Mesh*
Scene::getActive() {
  return m_meshs.at(m_active);
}

std::string
Scene::getActiveName()
{
  return m_active;
}

void
Scene::insertMesh(Mesh* mesh) {
  std::string name = "mesh" + std::to_string(m_size);
  m_meshs.insert({name, mesh});
  m_names.insert(name);
  m_active = name;
  m_size++;
}

void
Scene::removeMesh(std::string name) {
  m_size--;
  if (!m_size) {
    std::cout << "No more Mesh in Scene. Exiting" << std::endl;
    exit(-1);
  }
  m_meshs.erase(name);
  m_names.erase(name);
  m_active = *(--m_names.end());
}

void
Scene::createMesh (std::string name, std::string geomFile, std::string tex,
		   Material material, ShaderProgram& shader)
{
  auto mesh = new Mesh;
  AiScene Model (geomFile);
  mesh->addGeometry (Model.readVertexData (0));
  mesh->loadTexture (tex);
  mesh->setShaderPointer (& shader);
  mesh->setMaterial (material);
  mesh->activateMaterial ();
  mesh->prepareVao ();
  m_meshs.insert ({ name, mesh });
  m_names.insert (name);
  m_active = name;
  m_size++;
}

const Light&
Scene::getLight() const {
  return m_light;
}

void
Scene::setLight(const Light& light, ShaderProgram& shader) {
  m_light = light;
  shader.enable();
  GLint lightLoc = shader.getUniformLocation ("light.ambient");
  shader.setUniform3fv (lightLoc, 1, &m_light.ambient.x);
  lightLoc = shader.getUniformLocation ("light.diffuse");
  shader.setUniform3fv (lightLoc, 1, &m_light.diffuse.x);
  lightLoc = shader.getUniformLocation ("light.specular");
  shader.setUniform3fv (lightLoc, 1, &m_light.specular.x);
  lightLoc = shader.getUniformLocation ("light.direction");
  shader.setUniform3fv (lightLoc, 1, &m_light.direction.x);
  shader.disable();
}

void
Scene::draw(Camera cam) {
  float camTransform[16];
  cam.getTransform(camTransform);
  for (auto name : m_names) {
    auto* mesh = m_meshs.at(name);
    mesh->createModelViewMatrix(camTransform);
    mesh->draw();
  }
}

int
Scene::size() {
	return m_size;
}
