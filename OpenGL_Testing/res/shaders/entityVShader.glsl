 #version 330 core

in vec3 position;
in vec3 normal;
in vec2 texCoords;
in vec3 tangent;
in vec3 bitangent;


out vec3 toLightVector;
out vec3 surfaceNormal, p_tangent, p_bitangent;
out mat4 viewMat;
out vec3 LightDirection_tangentspace;
out vec3 EyeDirection_tangentspace;
out vec2 passTexCoords;
out mat3 TBN;
uniform mat4 translation;
uniform bool normal_mapping;


layout (std140) uniform camera_data
{ 
	mat4 projection;
	mat4 view;
};

layout (std140) uniform light{
	vec4 lightpos;
	vec4 lightColour;
};

layout (std140) uniform clip_plane{
	vec4 plane;
};

void main(){
	vec4 worldPos = translation * vec4(position, 1);
	gl_ClipDistance[0] = dot(worldPos, plane);
	toLightVector = normalize(lightpos.xyz - worldPos.xyz);
	passTexCoords = texCoords;
	gl_Position = projection * view * worldPos;

	vec3 LightDirection_cameraspace = normalize((view * lightpos).xyz - (view * worldPos).xyz);
	vec3 EyeDirection_cameraspace = normalize(vec3(0, 0, 0) - (view * worldPos).xyz);
	
	mat4 modelViewMatrix = view * translation;

	p_tangent = tangent;
	p_bitangent = bitangent;
	surfaceNormal = normal;

	TBN = transpose(mat3(
		modelViewMatrix * vec4(normalize(tangent), 0),
		modelViewMatrix * vec4(normalize(bitangent), 0),
		modelViewMatrix * vec4(normalize(normal), 0)
	)); 
	LightDirection_tangentspace = TBN * LightDirection_cameraspace;
	EyeDirection_tangentspace =  TBN * EyeDirection_cameraspace;
	viewMat = view;
}