#version 330

in vec2 vTexCoordOut;
in vec3 vNormalEyeOut;
in vec4 vPositionEyeOut;

out vec4 fragColor;

uniform sampler2D fSampler;
uniform float colorWeight = 0.0;

uniform vec3 ambientIntensity;
uniform vec3 emissiveIntensity;

// Represents a directional light
struct Light
{
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

uniform Light light = Light (vec3 (0.7, 0.7, 0.7),
			     vec3 (0.1, 0.1, 0.1), 
			     vec3 (0,0,1),
                             vec3 (0,0,0),
                             vec3 (0, 0, -1),
                             0.99, 15.0);

uniform Light dlight = Light(vec3(0.1,0.1,0.1),
			     vec3(0.1,0.1,0.1),
			     vec3(1,0,0),
			     vec3(0,0,0),
			     vec3(0,0,1),
			     0.0,0.0);

uniform Light plight = Light(vec3(0.1,0.1,0.1),
			     vec3(0.1,0.1,0.1),
			     vec3(0,1,0),
			     vec3(0,0,0),
			     vec3(0,0,0),
			     0.0,0.0);

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


void
main()
{

  vec3 normal=vNormalEyeOut;
  vec3 fragPos=vec3(vPositionEyeOut);  

  vec3 baseColor = vec3(0,0,0);

    //spotlight calculations
    vec3 lightVec = normalize( light.position - fragPos);
    float brightness = dot (normal, lightVec);
    brightness = max(brightness,0.0);
    vec3 diffuse = brightness * light.diffuse * material.diffuseRefl;

    vec3 specular = vec3(0.0);
    float spot = 1.0;
    float attenuation = 1.0;
    if (brightness > 0.0){

      vec3 reflectVec = reflect(-lightVec, normal);
      vec3 viewVec = normalize(-fragPos.xyz);
      float spec = dot (reflectVec, viewVec);
      spec = max(spec, 0.0);
      spec = pow (spec, material.specularPower);
      specular = spec * light.specular * material.specularRefl;

      //Non-directional, so light attenuates
      float distance = length (fragPos - light.position);
      attenuation = 1.0 / ( light.attenuationCoefficients.x * distance
          		  + light.attenuationCoefficients.y * distance
         		  + light.attenuationCoefficients.z * distance);

      vec3 spotDir = normalize (light.direction);
      spot =  dot(-lightVec, spotDir);
      spot = max(spot, 0.0);
      if (spot<light.cutoffCosAngle)
	spot = 0.0;
      spot = pow (spot, light.falloff);
    }

    baseColor+= spot*attenuation*(diffuse+specular);

    //direction light calculations
    lightVec = normalize (-light.direction);
    brightness = dot (normal, lightVec);
    brightness = max(brightness, 0.0);
    diffuse = brightness * light.diffuse * material.diffuseRefl;

    specular = vec3(0.0);
    if (brightness > 0.0){
      vec3 specularColor = material.specularRefl * dlight.specular;
      vec3 reflectVec = reflect (lightVec, normal);
      vec3 viewVec = normalize(-fragPos.xyz);
      float spec = dot (reflectVec, viewVec);
      spec = max( spec, 0.0);
      spec = pow ( spec, material.specularPower);
      specular = spec * light.specular * material.specularRefl;

    }

    //baseColor+= (diffuse+specular);

    //point light calculations
    lightVec = normalize (-light.direction);
    brightness = dot (normal, lightVec);
    brightness = max(brightness, 0.0);
    diffuse = brightness * light.diffuse * material.diffuseRefl;

    specular = vec3(0.0);
    attenuation = 0.0;
    if (brightness > 0.0){
      vec3 specularColor = material.specularRefl * dlight.specular;
      vec3 reflectVec = reflect (lightVec, normal);
      vec3 viewVec = normalize(-fragPos.xyz);
      float spec = dot (reflectVec, viewVec);
      spec = max( spec, 0.0);
      spec = pow ( spec, material.specularPower);
      specular = spec * light.specular * material.specularRefl;

      //Non-directional, so light attenuates
      float distance = length (fragPos - light.position);
      attenuation = 1.0 / ( light.attenuationCoefficients.x * distance
          		  + light.attenuationCoefficients.y * distance
         		  + light.attenuationCoefficients.z * distance);

    }

    //baseColor+= attenuation*(diffuse+specular);
    

    vec3 tex = vec3(texture(fSampler, vTexCoordOut));
    baseColor = spot*(diffuse+specular);
    baseColor = clamp (baseColor,0.0,1.0);
    fragColor = vec4(baseColor,1.0);
    fragColor = fragColor + mix(vec4(tex,1.0),vec4(0.1,0.1,0.1,1),.8);
            
}






















