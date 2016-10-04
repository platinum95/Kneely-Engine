
#version 330 core

in vec4 colourOffset;
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;  

in vec3 toLightVector;
in vec3 surfaceNormal;
in float pass_time;
in vec3 pass_colour;

layout (std140) uniform light{
	vec4 lightpos;
	vec4 lightColour;
};


void main(){

	vec3 unitNormal = surfaceNormal;
	vec3 unitLightVector = toLightVector;

	float timey = mod(pass_time / 12.0, 1.0);
	float nDotl = dot(unitNormal, unitLightVector);// * fade;
	float brightness = max(nDotl, 0.2);
	vec3 diffuse = vec3(brightness, brightness, brightness) * pass_colour;
	FragColor = vec4(diffuse, 1.0);
	BrightColor = vec4(0, 0, 0, 1);
	
}

