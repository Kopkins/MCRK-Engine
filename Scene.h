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

  const Light&
  getLight () const;

  void
  setLight (const Light& light, ShaderProgram& shader);

  void
  draw (Camera cam);

  int
  size ();

private:
  std::set<std::string> m_names;
  std::string m_active;
  std::unordered_map<std::string, Mesh*> m_meshs;
  Light m_light;
  int m_size;
};
