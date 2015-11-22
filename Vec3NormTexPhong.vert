#version 330

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec2 vTexCoord;

out vec3 vOutColor;
out vec2 vTexCoordOut;

uniform mat4 vModelView;
uniform mat4 vProjection;

// Represents a directional light
struct Light
{
  // These are ambient and diffuse light intensities
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  // Points toward the light source (which is infinitely far away)
  vec3 direction;
};

uniform Light light = Light (vec3 (0.2, 0.1, 0.6),
                             vec3 (0.1, 0.5, 0.7),
			     vec3 (0.1, 0.5, 0.7), 
			     vec3 (0, 0, 1));

struct Material
{
  // These are reflectances
  vec3 ambientRefl;
  vec3 diffuseRefl;
  vec3 specularRefl;
  float shininess;
};

uniform Material material;

//uniform Material material = Material (vec3 (0.1, 0.1, 0.2), 
//        	                      vec3 (0.75, 0.75, 0.8),
//                 		      vec3 (0.75, 0.75, 0.8), 16.0);

void
main ()
{
  vec4 positionEye = vModelView * vec4 (vPosition, 1.0);
  gl_Position = vProjection * positionEye;

  // Ambient lighting
  vec3 ambientColor = light.ambient * material.ambientRefl;

  // Diffuse lighting
  mat3 normalMatrix = mat3 (vModelView);
  // If non-uniform scaling is not used we can avoid the inverse transpose
  normalMatrix = transpose (inverse (normalMatrix));
  vec3 normalEye = normalize (normalMatrix * vNormal);
  float diffuseCoeff = dot (normalEye, normalize (light.direction));
  // It will be negative if the light is shining on the back side
  diffuseCoeff = max (diffuseCoeff, 0.0);
  vec3 diffuseColor = diffuseCoeff * light.diffuse * material.diffuseRefl;

  // Specular lighting
  vec3 specularColor = vec3 (0.0);
  if (diffuseCoeff > 0.0)
  {
    // Reflect incidence vector wrt normal
    vec3 reflection = reflect (-light.direction, normalEye);
    // Vector pointing toward the eye (0, 0, 0)
    vec3 viewDir = normalize (-positionEye.xyz);
    // Will be negative if eye is looking at the back side
    float specularCoeff = max (dot (reflection, viewDir), 0.0);
    specularCoeff = pow (specularCoeff, material.shininess);
    specularColor = specularCoeff * light.specular * material.specularRefl;
  }    
  
  vOutColor = ambientColor + diffuseColor + specularColor;
  vTexCoordOut = vTexCoord;
}
