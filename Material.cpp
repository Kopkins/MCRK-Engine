#include "Vector3.h"

struct Material{

  Material():
  ambientRefl(), diffuseRefl(), specularRefl(), shininess() {}

  Material(Vector3 ambientReflIn, Vector3 diffuseReflIn, Vector3 specularReflIn, float shininessIn):
    ambientRefl(ambientReflIn.x, ambientReflIn.y, ambientReflIn.z),
    diffuseRefl(diffuseReflIn.x, diffuseReflIn.y, diffuseReflIn.z),
    specularRefl(specularReflIn.x, specularReflIn.y, specularReflIn.z),
    shininess(shininessIn) {}

  Vector3 ambientRefl;
  Vector3 diffuseRefl;
  Vector3 specularRefl;
  float shininess;
};
