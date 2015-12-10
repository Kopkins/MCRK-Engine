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

  MeshNode (Mesh *);

  ~MeshNode ();

  Mesh * m_mesh;
  std::vector<Mesh *> m_children;
};



#endif /* MESHNODE_H_ */
