#include "Vector3.h"

struct Light{

  Light():
    diffuse(), specular(), position(), attenuationCoefficient(), direction(), cutoffCosAngle(),falloff() {}

  Light(Vector3 diffuseIn, Vector3 specularIn, Vector3 positionIn, Vector3 attenuationIn, Vector3 directionIn, float cutoffIn, float falloffIn):
  diffuse(diffuseIn.x, diffuseIn.y, diffuseIn.z),
  specular(specularIn.x, specularIn.y, specularIn.z),
  position(positionIn),
  attenuationCoefficient(attenuationIn),
  direction(directionIn.x, directionIn.y, directionIn.z),
  cutoffCosAngle(cutoffIn),
  falloff(falloffIn) {}

  Vector3 diffuse;
  Vector3 specular;
  Vector3 position;
  Vector3 attenuationCoefficient;
  Vector3 direction;
  float cutoffCosAngle;
  float falloff;
};
