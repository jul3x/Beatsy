#version 330 core

in vec2 UV;

out vec3 color;

uniform sampler2D renderedTexture;

vec2 blurSize = vec2(0.001, 0.001);

void main(){
   color = texture( renderedTexture, UV ).rgb;
   vec3 texColor = vec3(0.0); // texture2D(u_texture, vTexCoord)
   texColor += texture(renderedTexture, UV - 4.0*blurSize).rgb * 0.05;
   texColor += texture(renderedTexture, UV - 3.0*blurSize).rgb * 0.09;
   texColor += texture(renderedTexture, UV - 2.0*blurSize).rgb * 0.12;
   texColor += texture(renderedTexture, UV - blurSize).rgb * 0.15;
   texColor += texture(renderedTexture, UV).rgb * 0.16;
   texColor += texture(renderedTexture, UV + blurSize).rgb * 0.15;
   texColor += texture(renderedTexture, UV + 2.0*blurSize).rgb * 0.12;
   texColor += texture(renderedTexture, UV + 3.0*blurSize).rgb * 0.09;
   texColor += texture(renderedTexture, UV + 4.0*blurSize).rgb * 0.05;
   color = texColor;
}


