#version 330

//values brought in from program for use in shader
in vec2 vTexCoordOut;
in vec3 vNormalEyeOut;
in vec4 vPositionEyeOut;

//the color of the fragment to be returned
out vec4 fragColor;

//setting samplers for texture coords/mapping
uniform sampler2D dSampler;
uniform sampler2D sSampler;

//unused?
uniform float colorWeight = 0.0;

//map type
uniform float mapType = 0.0;

// Represents a directional light
struct Light
{
    // These are Light intensities
    vec3 diffuse;
    vec3 specular;

    //point and spotlight param
    vec3 position;
    vec3 attenuationCoefficients;

    // Points toward the Light source (which is infinitely far away)
    vec3 direction;

    //spotlight params
    float cutoffCosAngle;
    float falloff;

};

uniform int spotlightnum = 0;
uniform int directionlightnum = 0;
uniform int pointlightnum = 0;

//Light testLight = Light(vec3(spotlightnum,0,0),vec3(0.1,0.1,0.1),vec3(0,0,1),vec3(0.1,0.1,0.1),vec3(0,0,-1),.00,100);

uniform Light spotlights[10];
uniform Light directionlights[10];
uniform Light pointlights[10];

//material struct used for material reflectances
struct Material
{
    // These are reflectances
    vec3 ambientRefl;
    vec3 diffuseRefl;
    vec3 specularRefl;
    float specularPower;
};


//declaring a material
uniform Material material = Material (vec3 (1,1,1), 
        	                          vec3 (1,1,1),
                 		              vec3 (1,1,1), 222.0);


void
main()
{
 
    //assigning the fragments position and normal in eyespace 
    vec3 normal=vNormalEyeOut;
    vec3 fragPos=vec3(vPositionEyeOut);
 
 
    //setting base color for fragment and texture/
    vec3 baseColor = vec3(0,0,0);
    vec3 tex = vec3(texture(dSampler, vTexCoordOut));
    vec3 map = vec3(texture(sSampler, vTexCoordOut));

    //vector from light to fragment and brightness of the light
    vec3 lightVec = vec3(0.0);
    float brightness = 0.0;
  
    //specular and diffuse values of the light
    vec3 specular = vec3(0.0);
    vec3 diffuse = vec3(0.0);
  
    //used for "direction" of spot light
    float spot = 1.0;
  
    //used for light attenuation (strength fading over distance)
    float attenuation = 1.0;
    float distanceToLight = 0.0;
    
    /*****************************TestLight Calcluations Start****************************************************
        //Determining vector from light to fragment and brightness
        lightVec = normalize(testLight.position - fragPos);
        brightness = dot (normal, lightVec);
        brightness = max(brightness,0.0);

        //Resetting color components and setting diffuse light
        specular = vec3(0.0);
        diffuse = vec3(0.0);
        if (mapType==0.0)
            diffuse = brightness * testLight.diffuse * material.diffuseRefl;
        if (mapType!=0.0)
            diffuse = brightness * testLight.diffuse * tex;
        
        //setting values for light attenuation
        spot = 1.0;
        attenuation = 1.0;
        
        //specular light only calculated if brightness is over 0
        if (brightness > 0.0){

            //reflection of light from fragment
            vec3 reflectVec = reflect(-lightVec, normal);
            //vector from eye to fragment
            vec3 viewVec = normalize(-fragPos.xyz);
        
            //calculating a component of specular light using reflection and view
            float spec = dot (reflectVec, viewVec);
            //modifying specular light component using material
            spec = max(spec, 0.0);
            spec = pow (spec, material.specularPower);
        
            //setting specular light
            if (mapType!=2.0)
                specular = spec * testLight.specular * material.specularRefl;
            if (mapType==2.0)
                specular= spec * testLight.specular * map;

            //calculating light falloff based on cone cutoff 
            vec3 spotDir = normalize (testLight.direction);
            spot =  dot(-lightVec, spotDir);
            spot = max(spot, 0.0);
            if (spot<testLight.cutoffCosAngle)
                spot = 0.0;
            spot = pow (spot, testLight.falloff);
        }

        //add color changes for spotlight to fragment base color
        baseColor+= spot*attenuation*(diffuse+specular);
    
    *****************************TestLight Calcluations End****************************************************/
  
    /*****************************SpotLight Calcluations Start****************************************************/
    for (int i = 0; i<spotlightnum; i++){
        //Determining vector from light to fragment and brightness
        lightVec = normalize(spotlights[i].position - fragPos);
        brightness = dot (normal, lightVec);
        brightness = max(brightness,0.0);

        //Resetting color components and setting diffuse light
        specular = vec3(0.0);
        diffuse = vec3(0.0);
        if (mapType==0.0)
            diffuse = brightness * spotlights[i].diffuse * material.diffuseRefl;
        if (mapType!=0.0)
            diffuse = brightness * spotlights[i].diffuse * tex;
        
        //setting values for light attenuation
        spot = 1.0;
        attenuation = 1.0;
        
        //specular light only calculated if brightness is over 0
        if (brightness > 0.0){

            //reflection of light from fragment
            vec3 reflectVec = reflect(-lightVec, normal);
            //vector from eye to fragment
            vec3 viewVec = normalize(-fragPos.xyz);
        
            //calculating a component of specular light using reflection and view
            float spec = dot (reflectVec, viewVec);
            //modifying specular light component using material
            spec = max(spec, 0.0);
            spec = pow (spec, material.specularPower);
        
            //setting specular light
            if (mapType!=2.0)
                specular = spec * spotlights[i].specular * material.specularRefl;
            if (mapType==2.0)
                specular= spec * spotlights[i].specular * map;

            //calculating light falloff based on cone cutoff 
            vec3 spotDir = normalize (spotlights[i].direction);
            spot =  dot(-lightVec, spotDir);
            spot = max(spot, 0.0);
            if (spot<spotlights[i].cutoffCosAngle)
                spot = 0.0;
            spot = pow (spot, spotlights[i].falloff);
        }

        //add color changes for spotlight to fragment base color
        baseColor+= spot*attenuation*(diffuse+specular);
    }
    
    /*****************************SpotLight Calcluations End****************************************************/

    /*****************************Direction Light Calcluations Start********************************************/
    
    
    for (int i = 0; i<directionlightnum; i++){
    //Get light direction via inverse of vector to light
    lightVec = normalize (-directionlights[i].direction);
    brightness = dot (normal, lightVec);
    brightness = max(brightness, 0.0);
    
    //setting diffuse light
    if (mapType==0.0)
        diffuse = brightness * directionlights[i].diffuse * material.diffuseRefl;
    if (mapType!=0.0)
        diffuse = brightness * directionlights[i].diffuse * tex;

    //specular light only calculated if brightness is over 0
    if (brightness > 0.0){
        
        //unused?  
        //vec3 specularColor = material.specularRefl * directionlights[i].specular;
        //reflection of light from fragment
        vec3 reflectVec = reflect (lightVec, normal);
        //vector from eye to fragment
        vec3 viewVec = normalize(-fragPos.xyz);
      
        //calculating a component of specular light using reflection and view
        float spec = dot (reflectVec, viewVec);
        //modifying specular light component using material 
        spec = max( spec, 0.0);
        spec = pow ( spec, material.specularPower);
        
        //setting specular light
        if (mapType!=2.0)
            specular = spec * directionlights[i].specular * material.specularRefl;
        if (mapType==2.0)
            specular= spec * directionlights[i].specular * map;

    }
    
    baseColor+= (diffuse+specular);
    }
    
   /*****************************Direction Light Calcluations End**************************************************/

    /*****************************Point Light Calcluations Start***************************************************/
    
    for(int i=0; i<pointlightnum; i++){
    
    //Get light direction using position and fragment position
    lightVec = normalize ( pointlights[i].position - fragPos);
    brightness = dot (normal, lightVec);
    brightness = max(brightness, 0.0);
    
    //setting diffuse light
    if (mapType==0.0)
      diffuse = brightness * pointlights[i].diffuse * material.diffuseRefl;
    //if (mapType!=0.0)
      //diffuse = brightness * pointlights[i].diffuse * tex;

    //setting attenuation
    attenuation = 1.0;
    //specular light only calculated if brightness is over 0
    if (brightness > 0.0){
      //unused?  
      vec3 specularColor = material.specularRefl * pointlights[i].specular;
      //get light reflection and view vectors
      vec3 reflectVec = reflect (lightVec, normal);
      vec3 viewVec = normalize(-fragPos.xyz);
      //calculated specular light component with reflect and view vectors
      float spec = dot (reflectVec, viewVec);
      spec = max( spec, 0.0);
      spec = pow ( spec, material.specularPower);
      //setting specular light
      if (mapType!=2.0)
        specular = spec * pointlights[i].specular * material.specularRefl;
      if (mapType==2.0)
        specular= spec * pointlights[i].specular * map;

      //calculating light attenuation based on distance to light
      distanceToLight = length (fragPos - pointlights[i].position);
      attenuation = 1.0 / ( pointlights[i].attenuationCoefficients.x * distanceToLight
          		  + pointlights[i].attenuationCoefficients.y * distanceToLight
         		  + pointlights[i].attenuationCoefficients.z * distanceToLight);

    }

    baseColor+= attenuation*(diffuse+specular);
    
    }

    /*****************************Point Light Calcluations End**************************************************/
    
    
    baseColor = clamp (baseColor,0.0,1.0);
    fragColor = vec4(baseColor,1.0);
            
}






















