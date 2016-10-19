
#version 330 core

in vec4 colourOffset;
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;  

in mat3 TBN;
in vec3 toLightVector;
in vec3 surfaceNormal,  p_tangent, p_bitangent;
in vec2 passTexCoords;
in vec3 LightDirection_tangentspace, EyeDirection_tangentspace;
in mat4 viewMat;
uniform sampler2D diffuseTex, normalTex;

layout (std140) uniform light{
	vec4 lightpos;
	vec4 lightColour;
};


void main(){

	vec3 unitNormal = surfaceNormal;
	vec3 unitLightVector = toLightVector;
	vec3 TextureNormal_tangentspace = normalize(texture( normalTex, passTexCoords ).rgb*2.0 - 1.0);
	vec3 R = reflect(-LightDirection_tangentspace,TextureNormal_tangentspace);

	float cosAlpha = clamp( dot( EyeDirection_tangentspace, R ), 0,1 );
	float nDotl = dot(TextureNormal_tangentspace, LightDirection_tangentspace);// * fade;
	float brightness = max(nDotl, 0.4);
	vec4 specularComp = vec4(1, 1, 1, 1) * lightColour * 1 * pow(cosAlpha, 5);
	vec3 diffuse = vec3(brightness, brightness, brightness);
	FragColor = vec4(diffuse, 1.0) * texture(diffuseTex, passTexCoords) + specularComp;
	BrightColor = vec4(0, 0, 0, 1);
	
}
