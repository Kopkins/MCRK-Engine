#include <string>
#include <unordered_map>
#include <set>
#include <memory>
#include "AiScene.h"
#include "Mesh.h"
#include "VertexBuffer.h"
#include "Light.cpp"

using Geometry = std::shared_ptr<VertexBuffer>;

class Scene {
public:
	Scene ();

	void
	setActive(std::string name);

	Mesh*
	getMesh(std::string name);

	Mesh*
	getActive();

	void
	setLight(Light light);

	void
	createMesh(std::string name);

	void
	createMesh(std::string name, std::string geomFile,
			std::string tex, Material material,
			ShaderProgram* shader);

	const Light& getLight() const;

	void setLight(const Light& light);

private:
	std::set<std::string> m_names;
	std::string m_active;
	std::unordered_map<std::string, Mesh*> m_meshs;
	Light m_light;
};
