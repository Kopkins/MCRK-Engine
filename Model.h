/*
 * Model.h
 *
 *  Created on: Dec 9, 2015
 *      Author: Kyle Hopkins
 */

#ifndef MODEL_H_
#define MODEL_H_

#include "MeshNode.cpp"
#include "Mesh.h"

class Model {
public:
  Model ();

  ~Model ();

  Model (const MeshNode node);

  void
  draw ();

  void
  setRoot (const MeshNode node);

  std::vector<MeshNode *>&
  getChildren ();

private:
  MeshNode m_root;

};




#endif /* MODEL_H_ */
