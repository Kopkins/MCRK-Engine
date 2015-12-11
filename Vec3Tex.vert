#version 330

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec2 vTexCoord;

out vec2 vTexCoordOut;
out vec3 vNormalEyeOut;
out vec4 vPositionEyeOut;

uniform mat4 vModelView;
uniform mat4 vProjection;

void
main()
{

vec4 eyePosition = vModelView * vec4 (vPosition, 1.0);
gl_Position = vProjection * eyePosition;
eyePosition = vModelView * vec4 (vPosition, 1.0);

  mat3 normalMatrix = mat3 (vModelView);
  // If non-uniform scaling is not used we can avoid the inverse transpose
  normalMatrix = transpose (inverse (normalMatrix));
  vec3 normalEye = normalize (normalMatrix * vNormal);

vTexCoordOut=vTexCoord;
vNormalEyeOut=normalEye;
vPositionEyeOut=eyePosition;
}
