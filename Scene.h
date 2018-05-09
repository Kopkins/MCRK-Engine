#include <string>
#include <unordered_map>
#include <set>
#include <memory>
#include "AiScene.h"
#include "Mesh.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Light.cpp"
#include "Camera.h"
#include "ShaderProgram.h"

using Geometry = std::shared_ptr<VertexBuffer>;

class Scene
{
public:
  Scene ();

  void
  setActive (std::string name);

  Mesh*
  getMesh (std::string name);

  Mesh*
  getActive ();

  std::string
  getActiveName ();

  void
  insertMesh (Mesh* mesh);

  void
  removeMesh (std::string name);

  void
  createMesh (std::string name, std::string geomFile, std::string tex,
	      Material material, ShaderProgram& shader, int index=0);

  void
  addMesh (Mesh newMesh);

  void
  createLight (std::string name, std::string type, Vector3 diffuse, Vector3 specular, Vector3 position,
        Vector3 attenuationCoefficient, Vector3 direction, float cutoffCosAngle,
        float falloff);

  void
  createLight(std::string name, Light* Light);

  void
  addLight (const Light& light);

  const Light&
  getLight () const;

  void
  setLights (ShaderProgram& shader);

  void
  draw (Camera cam);

  int
  size ();

private:
  std::set<std::string> m_names;
  std::string m_active;
  std::unordered_map<std::string, Mesh*> m_meshes;
  std::unordered_map<std::string, Light*> m_lights;
  Light m_light;
  int m_size;
};
