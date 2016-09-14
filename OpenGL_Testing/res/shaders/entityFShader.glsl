
#version 330 core

in vec4 colourOffset;
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;  

in vec3 toLightVector;
in vec3 surfaceNormal;
in vec2 passTexCoords;

uniform sampler2D diffuseTex;

layout (std140) uniform light{
	vec4 lightpos;
	vec4 lightColour;
};


void main(){

	vec3 unitNormal = surfaceNormal;
	vec3 unitLightVector = toLightVector;

	
	float nDotl = dot(unitNormal, unitLightVector);// * fade;
	float brightness = max(nDotl, 0.2);
	vec3 diffuse = vec3(brightness, brightness, brightness);
	FragColor = vec4(diffuse, 1.0) * texture(diffuseTex, passTexCoords);
	BrightColor = vec4(0, 0, 0, 1);
	
}
