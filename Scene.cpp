#include "Scene.h"

Scene::Scene() :
		m_names(), m_active(), m_meshes(), m_lights(), m_size(0) {
}

void
Scene::setActive(std::string name) {
  if (m_names.count(name))
    m_active = name;
}

Mesh*
Scene::getMesh(std::string name) {
  return m_meshes.at(name);
}


Mesh*
Scene::getActive() {
  return m_meshes.at(m_active);
}

std::string
Scene::getActiveName()
{
  return m_active;
}

void
Scene::insertMesh(Mesh* mesh) {
  std::string name = "mesh" + std::to_string(m_size);
  m_meshes.insert({name, mesh});
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
  m_meshes.erase(name);
  m_names.erase(name);
  m_active = *(--m_names.end());
}

void
Scene::createMesh (std::string name, std::string geomFile, std::string tex,
		   Material material, ShaderProgram& shader, int index)
{
  auto mesh = new Mesh;
  AiScene Model (geomFile);
  std::shared_ptr<VertexBuffer> vBuf (new VertexBuffer);
  std::shared_ptr<IndexBuffer> iBuf (new IndexBuffer);
  Model.readBufferData (index, vBuf, iBuf);
  mesh->addGeometry (vBuf);
  mesh->addIndices (iBuf);
  mesh->loadTexture (0,tex);
  mesh->loadTexture(1,"sh3_s.jpg");
  mesh->setShaderPointer (& shader);
  mesh->setMaterial (material);
  mesh->activateMaterial ();
  mesh->prepareVao ();
  m_meshes.insert ({ name, mesh });
  m_names.insert (name);
  m_active = name;
  m_size++;
}

void Scene::createLight (std::string name, std::string type, Vector3 diffuse, Vector3 specular, Vector3 position,
        Vector3 attenuationCoefficient, Vector3 direction, float cutoffCosAngle,
        float falloff)
{
    auto light = new Light(type, diffuse,specular,position,attenuationCoefficient,direction,cutoffCosAngle,falloff);
    m_lights.insert({name,light});
}

void Scene::createLight (std::string name, Light* light){
    m_lights.insert({name,light});
}

const Light&
Scene::getLight() const {
  return m_light;
}

void
Scene::setLights(ShaderProgram& shader) {
  shader.enable();

  GLint lightLoc;
  int spotcount = 0;
  int directioncount = 0;
  int pointcount = 0;

  for (auto index = m_lights.begin(); index != m_lights.end(); ++index){
    if (index->second->type=="spot"){
        std::cout << "is a spot light\n";
        lightLoc = shader.getUniformLocation ("spotlights["+std::to_string(spotcount)+std::string("].diffuse"));
        shader.setUniform3fv (lightLoc, 1, &index->second->diffuse.x);
        lightLoc = shader.getUniformLocation ("spotlights["+std::to_string(spotcount)+std::string("].specular"));
        shader.setUniform3fv (lightLoc, 1, &index->second->specular.x);
        lightLoc = shader.getUniformLocation ("spotlights["+std::to_string(spotcount)+std::string("].position"));
        shader.setUniform3fv (lightLoc, 1, &index->second->position.x);
        lightLoc = shader.getUniformLocation("spotlights["+std::to_string(spotcount)+std::string("].attenuationCoefficients"));
        shader.setUniform3fv (lightLoc, 1, &index->second->attenuationCoefficient.x);
        lightLoc = shader.getUniformLocation("spotlights["+std::to_string(spotcount)+std::string("].direction"));
        shader.setUniform3fv (lightLoc, 1, &index->second->direction.x);
        lightLoc = shader.getUniformLocation("spotlights["+std::to_string(spotcount)+std::string("].cutoffCosAngle"));
        shader.setUniform1f (lightLoc, index->second->cutoffCosAngle);
        lightLoc = shader.getUniformLocation("spotlights["+std::to_string(spotcount)+std::string("].falloff"));
        shader.setUniform1f (lightLoc, index->second->falloff);
        spotcount++;
    }
    

    else if (index->second->type=="direction"){
        std::cout << "is a direction light\n";
        lightLoc = shader.getUniformLocation ("directionlights["+std::to_string(directioncount)+std::string("].diffuse"));
        shader.setUniform3fv (lightLoc, 1, &index->second->diffuse.x);
        lightLoc = shader.getUniformLocation ("directionlights["+std::to_string(directioncount)+std::string("].specular"));
        shader.setUniform3fv (lightLoc, 1, &index->second->specular.x);
        lightLoc = shader.getUniformLocation ("directionlights["+std::to_string(directioncount)+std::string("].position"));
        shader.setUniform3fv (lightLoc, 1, &index->second->position.x);
        lightLoc = shader.getUniformLocation("directionlights["+std::to_string(directioncount)+std::string("].attenuationCoefficients"));
        shader.setUniform3fv (lightLoc, 1, &index->second->attenuationCoefficient.x);
        lightLoc = shader.getUniformLocation("directionlights["+std::to_string(directioncount)+std::string("].direction"));
        shader.setUniform3fv (lightLoc, 1, &index->second->direction.x);
        lightLoc = shader.getUniformLocation("directionlights["+std::to_string(directioncount)+std::string("].cutoffCosAngle"));
        shader.setUniform1f (lightLoc, index->second->cutoffCosAngle);
        lightLoc = shader.getUniformLocation("directionlights["+std::to_string(directioncount)+std::string("].falloff"));
        shader.setUniform1f (lightLoc, index->second->falloff);
        directioncount++;
    }
    
    else if (index->second->type=="point"){
        std::cout << "is a point light\n";
        lightLoc = shader.getUniformLocation ("pointlights["+std::to_string(pointcount)+std::string("].diffuse"));
        shader.setUniform3fv (lightLoc, 1, &index->second->diffuse.x);
        lightLoc = shader.getUniformLocation ("pointlights["+std::to_string(pointcount)+std::string("].specular"));
        shader.setUniform3fv (lightLoc, 1, &index->second->specular.x);
        lightLoc = shader.getUniformLocation ("pointlights["+std::to_string(pointcount)+std::string("].position"));
        shader.setUniform3fv (lightLoc, 1, &index->second->position.x);
        lightLoc = shader.getUniformLocation("pointlights["+std::to_string(pointcount)+std::string("].attenuationCoefficients"));
        shader.setUniform3fv (lightLoc, 1, &index->second->attenuationCoefficient.x);
        lightLoc = shader.getUniformLocation("pointlights["+std::to_string(pointcount)+std::string("].direction"));
        shader.setUniform3fv (lightLoc, 1, &index->second->direction.x);
        lightLoc = shader.getUniformLocation("pointlights["+std::to_string(pointcount)+std::string("].cutoffCosAngle"));
        shader.setUniform1f (lightLoc, index->second->cutoffCosAngle);
        lightLoc = shader.getUniformLocation("pointlights["+std::to_string(pointcount)+std::string("].falloff"));
        shader.setUniform1f (lightLoc, index->second->falloff);
        pointcount ++;
    }
  }


    GLint countLoc = shader.getUniformLocation("spotlightnum");
    std::cout << countLoc << " - " << spotcount <<"\n";
    shader.setUniform1i(countLoc, spotcount);
    countLoc = shader.getUniformLocation("directionlightnum");
    std::cout << countLoc << " - " << directioncount <<"\n";
    shader.setUniform1i(countLoc, directioncount);
    countLoc = shader.getUniformLocation("pointlightnum");
    std::cout << countLoc << " - " << pointcount <<"\n";
    shader.setUniform1i(countLoc, pointcount);

  shader.disable();
}



void
Scene::draw(Camera cam) {
  float camTransform[16];
  cam.getTransform(camTransform);
  for (auto name : m_names) {
    auto mesh = m_meshes.at(name);
    mesh->createModelViewMatrix(camTransform);
    mesh->draw();
  }
}

int
Scene::size() {
	return m_size;
}
