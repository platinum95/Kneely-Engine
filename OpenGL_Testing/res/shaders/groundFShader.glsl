
#version 330 core

out vec4 FragColor;

uniform sampler2D tex, normalTex;
in vec2 texCoords;
in vec4 vertPos;
in vec3 toLightVector;
in vec3 surfaceNormal;

flat in int clipp;

layout (std140) uniform light{
	vec3 lightpos;
	vec3 lightColour;
};

vec3 getNormal(vec2 _coord);

void main(){
	
	vec4 normalMapValue = (texture(normalTex, texCoords) + 1.0) / 2.0;
	vec3 unitNormal = normalize(surfaceNormal);
	vec3 unitLightVector = normalize(toLightVector);
	
	float nDotl = dot(unitNormal, unitLightVector);
	float brightness = max(nDotl, 0.4);
	vec3 diffuse = vec3(brightness, brightness, brightness);// * lightColour;
	vec3 normal = getNormal(texCoords);

	if(clipp == 1){
		FragColor = vec4(0, 0.5, 0, 1);
	}
	else{
		FragColor = vec4(diffuse, 1.0) * texture(tex, texCoords);
	}
	
	
}

vec3 getNormal(vec2 _coord){
	vec3 normal = vec3(texture(normalTex, _coord));
	normal.r = normal.r * 2.0 - 1.0;
	normal.g = normal.g * 2.0 - 1.0;
	normal.b = normal.b * 2.0 - 1.0;
	return normalize(normal);
}
