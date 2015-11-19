#include "Vector3.h"

struct Light{

  Light():
  ambient(), diffuse(), specular(), direction() {}

  Light(Vector3 ambientIn, Vector3 diffuseIn, Vector3 specularIn, Vector3 directionIn):
  ambient(ambientIn.x, ambientIn.y, ambientIn.z),
  diffuse(diffuseIn.x, diffuseIn.y, diffuseIn.z),
  specular(specularIn.x, specularIn.y, specularIn.z),
  direction(directionIn.x, directionIn.y, directionIn.z) {}

  Vector3 ambient;
  Vector3 diffuse;
  Vector3 specular;
  Vector3 direction;
};
