
#version 330 core

out vec4 FragColor;

uniform sampler2D tex;
in vec2 texCoords;
in vec4 vertPos;
in vec3 toLightVector;
in vec3 surfaceNormal;

flat in int clipp;

layout (std140) uniform light{
	vec3 lightpos;
	vec3 lightColour;
};

void main(){

	vec3 unitNormal = normalize(surfaceNormal);
	vec3 unitLightVector = normalize(toLightVector);
	
	float nDotl = dot(unitNormal, unitLightVector);
	float brightness = max(nDotl, 0.4);
	vec3 diffuse = vec3(brightness, brightness, brightness);// * lightColour;
	
	if(clipp == 1){
		FragColor = vec4(0, 0.5, 0, 1);
	}
	else{
		FragColor = vec4(diffuse, 1.0) * texture(tex, texCoords);
	}
	
	
}
