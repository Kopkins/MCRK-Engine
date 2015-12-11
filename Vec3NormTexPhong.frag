#version 330

in vec2 vTexCoordOut;
in vec3 vNormalEyeOut;
in vec4 vPositionEyeOut;

out vec4 fragColor;

uniform sampler2D fSampler;
uniform float colorWeight = 0.5;

uniform vec3 ambientIntensity;
uniform vec3 emissiveIntensity;

// Represents a directional light
struct Light
{
  // 0 if directional, 1 if point, 2 if spot
  int type;

  // These are light intensities
  vec3 diffuse;
  vec3 specular;

  //point and spotlight params
  vec3 position;
  vec3 attenuationCoefficients;

  // Points toward the light source (which is infinitely far away)
  vec3 direction;

  //spotlight params
  float cutoffCosAngle;
  float falloff;

};

uniform Light light = Light (2,vec3 (0.5, 0.5, 0.5),
			     vec3 (0.5, 0.5, 0.5), 
			     vec3 (0,0,0),
                             vec3 (0,0,0),
                             vec3 (0, 0, -1),
                             0.99, 15.0);

struct Material
{
  // These are reflectances
  vec3 ambientRefl;
  vec3 diffuseRefl;
  vec3 specularRefl;
  float specularPower;
};


uniform Material material = Material (vec3 (1,1,1), 
        	                      vec3 (1, 1, 1),
                 		      vec3 (1, 1, 1), 222.0);


vec3 calculateLighting (Light light, Material material, vec3 vertexPosition, vec3 vertexNormal);

void
main()
{
    
    //vec3 baseColor=material.ambientRefl*ambientIntensity*emissiveIntensity;
    //vec3 baseColor=vec3(1,1,1);


  vec3 normal=vNormalEyeOut;
  vec3 fragPos=vec3(vPositionEyeOut);
  
 // for (int i=0; i<1; ++i)
 // {
 //   baseColor+=calculateLighting (light, material, fragPos ,normal);
 // }

    vec3 lightVec = normalize( light.position - fragPos);
    float brightness = dot (normal, lightVec);
    brightness = max(brightness,0.0);
    vec3 diffuse = brightness * light.diffuse * material.diffuseRefl;

    vec3 specular = vec3(0.0);
    float spot = 1.0;
    if (brightness > 0.0){
      vec3 reflectVec = reflect(-lightVec, normal);
      vec3 viewVec = normalize(-fragPos.xyz);
      float spec = dot (reflectVec, viewVec);
      spec = max(spec, 0.0);
      spec = pow (spec, material.specularPower);
      specular = spec * light.specular * material.specularRefl;

      vec3 spotDir = normalize (light.direction);
      spot =  dot(-lightVec, spotDir);
      spot = max(spot, 0.0);
      if (spot<light.cutoffCosAngle)
	spot = 0.0;
      spot = pow (spot, light.falloff);
    }

      vec3 baseColor = spot*(diffuse+specular);
      baseColor = clamp (baseColor,0.0,1.0);
      
      vec4 textureColor = texture(fSampler, vTexCoordOut);
      fragColor = mix (textureColor, vec4(baseColor,1.0), colorWeight);
      

    //vec4 lightColor=vec4(baseColor,1);
    //lightColor=clamp(fragColor, 0.0, 1.0);
    //vec4 textureColor = texture(fSampler, vTexCoordOut);
    //fragColor = mix(textureColor, lightColor, colorWeight);
    //fragColor=lightColor;
}






















