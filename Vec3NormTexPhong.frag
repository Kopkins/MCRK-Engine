#version 330

in vec2 vTexCoordOut;
in vec3 vNormalEyeOut;
in vec4 vPositionEyeOut;

out vec4 fColor;

uniform sampler2D fSampler;
uniform float colorWeight = 0.0;

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


uniform Material material = Material (vec3 (0.1, 0.1, 0.2), 
        	                      vec3 (0.75, 0.75, 0.8),
                 		      vec3 (0.75, 0.75, 0.8), 16.0);

void
main()
{

  // Ambient lighting
  vec3 ambientColor = light.ambient * material.ambientRefl;

  vec3 normalEye=vNormalEyeOut;
  float diffuseCoeff = dot (normalEye, normalize (light.direction));
  // It will be negative if the light is shining on the back side
  diffuseCoeff = max (diffuseCoeff, 0.0);
  vec3 diffuseColor = diffuseCoeff * light.diffuse * material.diffuseRefl;

  // Specular lighting
  vec3 specularColor = vec3 (0.0);
  vec4 positionEye=vPositionEyeOut;
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
  
   vec3 lightColor = ambientColor + diffuseColor + specularColor;

   vec4 textureColor = texture (fSampler, vTexCoordOut);
   fColor = mix (textureColor, vec4 (lightColor, 1), colorWeight);
}
