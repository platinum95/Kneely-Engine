#version 330 

in vec3 textureCoords;

layout (location = 0) out vec4 out_Color;
layout (location = 1) out vec4 BrightColor;  

uniform samplerCube cubeMap;

void main(void){
    out_Color = texture(cubeMap, textureCoords);
	BrightColor = vec4(0, 0, 0, 1);
}
