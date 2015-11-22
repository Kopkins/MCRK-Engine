#include "Scene.h"

Scene::Scene() :
		m_names(), m_active(), m_meshs() {
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

void
Scene::setLight(Light light){
	m_light = light;
}

void
Scene::createMesh(std::string name) {
	m_meshs.insert({name, new Mesh});
}

void
Scene::createMesh(std::string name, std::string geomFile,
					std::string tex, Material material,
					ShaderProgram* shader) {

	auto mesh = new Mesh;
	AiScene Model (geomFile);
	mesh->addGeometry(Model.readVertexData (0));
	mesh->loadTexture(tex);
	mesh->setShaderPointer(shader);
	mesh->setMaterial(material);
	mesh->activateMaterial();
	mesh->prepareVao();
	m_meshs.insert({name, mesh});
	m_names.insert(name);
	m_active = name;
}

const Light& Scene::getLight() const {
	return m_light;
}

void Scene::setLight(const Light& light) {
	m_light = light;
}
