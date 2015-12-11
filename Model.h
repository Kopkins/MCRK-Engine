/*
 * Model.h
 *
 *  Created on: Dec 9, 2015
 *      Author: Kyle Hopkins
 */

#ifndef MODEL_H_
#define MODEL_H_

#include <vector>
#include <string>
#include "Mesh.h"

class Model {
public:
  Model ();

  ~Model ();


  void
  draw ();

  void
  addMesh (const Mesh& mesh);

private:
  std::vector<Mesh *> m_meshes;

};




#endif /* MODEL_H_ */
