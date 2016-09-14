
#version 330 core

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;  

uniform sampler2D tex;
in vec2 texCoords;
in vec4 vertPos;
in vec3 toLightVector;
in vec3 surfaceNormal;
in float fade;

flat in int clipp;

layout (std140) uniform light{
	vec4 lightpos;
	vec4 lightColour;
};


void main(){

	
	vec3 unitNormal = surfaceNormal;
	vec3 unitLightVector = toLightVector;

	
	float nDotl = dot(unitNormal, unitLightVector);// * fade;
	float brightness = max(nDotl, 0.2);
	vec3 diffuse = lightColour.xyz * brightness;
	if(clipp == 1){
		FragColor = vec4(0, 0, 0, 1);
	}
	else{
		FragColor = vec4(diffuse, 1.0) * texture(tex, texCoords);
	}
	float brightness2 = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
    
	if(brightness2 > 0.6)
        BrightColor = vec4(FragColor.rgb, 1.0);
	else
		BrightColor = vec4(0, 0, 0, 1);
	
}
