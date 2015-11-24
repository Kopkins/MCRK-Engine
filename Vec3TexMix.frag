#version 330

in vec3 vOutColor;
in vec2 vTexCoordOut;

out vec4 fColor;

uniform sampler2D fSampler;
uniform float colorWeight = 0.0;

void
main ()
{
  vec4 textureColor = texture (fSampler, vTexCoordOut); 
  fColor = mix (textureColor, vec4 (vOutColor, 1), colorWeight);
}
