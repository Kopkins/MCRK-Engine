#version 330

in vec2 vTexCoordOut;
in vec3 vNormalEyeOut;
in vec4 vPositionEyeOut;

out vec4 fragColor;

uniform sampler2D dSampler;
uniform sampler2D sSampler;
uniform float colorWeight = 0.0;

uniform float mapType;

// Represents a directional light
struct Light
{
  // These are Light intensities
  vec3 diffuse;
  vec3 specular;

  //point and spotlight params
  vec3 position;
  vec3 attenuationCoefficients;

  // Points toward the Light source (which is infinitely far away)
  vec3 direction;

  //spotlight params
  float cutoffCosAngle;
  float falloff;

};


uniform Light ambientLight = Light (vec3 (1, 1, 1),
			     vec3 (0.1, 0.1, 0.1), 
			     vec3 (0,0,1),
                 vec3 (0.1,0.1,0.1),
                 vec3 (0, 0, -1),
                 0, 0);

uniform Light dlight = Light(vec3(1,1,1),
			     vec3(0.1,0.1,0.1),
			     vec3(0,0,0),
			     vec3(0,0,0),
			     vec3(1,0,0),
			     0.0,0.0);

uniform Light plight = Light(vec3(0,1,0),
			     vec3(0.05,0.05,0.05),
			     vec3(0,1,0),
			     vec3(0.1,0.1,0.1),
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
        	                          vec3 (1,1,1),
                 		              vec3 (1,1,1), 222.0);


void
main()
{

  vec3 normal=vNormalEyeOut;
  vec3 fragPos=vec3(vPositionEyeOut);  

  vec3 baseColor = vec3(0,0,0);
  vec3 tex = vec3(texture(dSampler, vTexCoordOut));
  vec3 map = vec3(texture(sSampler, vTexCoordOut));

  vec3 lightVec = vec3(0.0);
  float brightness = 0.0;
  
  vec3 specular = vec3(0.0);
  vec3 diffuse = vec3(0.0);
  
  float spot = 1.0;
  float attenuation = 1.0;
  float distanceToLight = 0.0;
  
    
    /*spotlight calculations
    lightVec = normalize(ambientLight.position - fragPos);
    brightness = dot (normal, lightVec);
    brightness = max(brightness,0.0);

    specular = vec3(0.0);
    diffuse = vec3(0.0);
    if (mapType==0.0)
      diffuse = brightness * ambientLight.diffuse * material.diffuseRefl;
    if (mapType!=0.0)
      diffuse = brightness * ambientLight.diffuse * tex;
      

    spot = 1.0;
    attenuation = 1.0;
    if (brightness > 0.0){

      vec3 reflectVec = reflect(-lightVec, normal);
      vec3 viewVec = normalize(-fragPos.xyz);
      float spec = dot (reflectVec, viewVec);
      spec = max(spec, 0.0);
      spec = pow (spec, material.specularPower);
      if (mapType!=2.0)
        specular = spec * ambientLight.specular * material.specularRefl;
      if (mapType==2.0)
        specular= spec * ambientLight.specular * map;

      //Non-directional, so Light attenuates
      distanceToLight = length (fragPos - ambientLight.position);
      attenuation = 1.0 / ( ambientLight.attenuationCoefficients.x * distanceToLight
          		  + ambientLight.attenuationCoefficients.y * distanceToLight
        		  + ambientLight.attenuationCoefficients.z * distanceToLight);
        		  
        		  
      vec3 spotDir = normalize (ambientLight.direction);
      spot =  dot(-lightVec, spotDir);
      spot = max(spot, 0.0);
      if (spot<ambientLight.cutoffCosAngle)
	spot = 0.0;
      spot = pow (spot, ambientLight.falloff);
    }

    baseColor+= spot*attenuation*(diffuse+specular);
 */    
    

    /*direction Light calculations
    lightVec = normalize (-dlight.direction);
    brightness = dot (normal, lightVec);
    brightness = max(brightness, 0.0);
    if (mapType==0.0)
      diffuse = brightness * dlight.diffuse * material.diffuseRefl;
    //if (mapType!=0.0)
      //diffuse = brightness * dlight.diffuse * tex;

    if (brightness > 0.0){
      vec3 specularColor = material.specularRefl * dlight.specular;
      vec3 reflectVec = reflect (lightVec, normal);
      vec3 viewVec = normalize(-fragPos.xyz);
      float spec = dot (reflectVec, viewVec);
      spec = max( spec, 0.0);
      spec = pow ( spec, material.specularPower);
      if (mapType!=2.0)
        specular = spec * dlight.specular * material.specularRefl;
      if (mapType==2.0)
        specular= spec * dlight.specular * map;

    }
    baseColor+= (diffuse+specular);
    
    */

    //point Light calculations
    lightVec = normalize ( plight.position - fragPos);
    brightness = dot (normal, lightVec);
    brightness = max(brightness, 0.0);
    if (mapType==0.0)
      diffuse = brightness * plight.diffuse * material.diffuseRefl;
    //if (mapType!=0.0)
      //diffuse = brightness * plight.diffuse * tex;

    attenuation = 1.0;
    if (brightness > 0.0){
      vec3 specularColor = material.specularRefl * dlight.specular;
      vec3 reflectVec = reflect (lightVec, normal);
      vec3 viewVec = normalize(-fragPos.xyz);
      float spec = dot (reflectVec, viewVec);
      spec = max( spec, 0.0);
      spec = pow ( spec, material.specularPower);
      if (mapType!=2.0)
        specular = spec * plight.specular * material.specularRefl;
      if (mapType==2.0)
        specular= spec * plight.specular * map;

      //Non-directional, so Light attenuates
      distanceToLight = length (fragPos - plight.position);
      attenuation = 1.0 / ( plight.attenuationCoefficients.x * distanceToLight
          		  + plight.attenuationCoefficients.y * distanceToLight
         		  + plight.attenuationCoefficients.z * distanceToLight);

    }

    baseColor+= attenuation*(diffuse+specular);
    
    //*/
    
    baseColor = clamp (baseColor,0.0,1.0);
    fragColor = vec4(baseColor,1.0);
            
}






















