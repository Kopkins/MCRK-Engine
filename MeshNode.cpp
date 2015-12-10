/*
 * MeshNode.h
 *
 *  Created on: Dec 9, 2015
 *      Author: Kyle Hopkins
 */

#ifndef MESHNODE_H_
#define MESHNODE_H_

#include <vector>
#include "Mesh.h"

struct MeshNode {

  MeshNode (Mesh * mesh) : m_mesh(mesh) {}

  ~MeshNode () {}

  void
  draw ()
  {
    m_mesh->draw ();
    for (auto* mesh : m_children) {
	mesh->draw ();
    }
  }

  Mesh * m_mesh;
  std::vector<MeshNode *> m_children;
};



#endif /* MESHNODE_H_ */
