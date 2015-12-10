#include "Model.h"

Model::Model () : m_root(NULL) {}

Model::~Model ()
{
}

Model::Model (const MeshNode node) : m_root(node) {}

void
Model::draw ()
{
  m_root.draw ();
}

void
Model::setRoot (const MeshNode node)
{
  m_root = node;
}

std::vector<MeshNode *>&
Model::getChildren () {
  return m_root.m_children;
}

void
Model::addChild (const Mesh& mesh)
{
  auto* node  = new MeshNode(mesh);
  m_root.m_children.push_back(node);
}
