
#version 330 core

in vec3 position;
in vec3 normal;
in vec2 texCoords;


out vec3 toLightVector;
out vec3 surfaceNormal;

out vec2 passTexCoords;

uniform mat4 translation;
uniform float test;


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
	surfaceNormal = normalize((translation * vec4(normal, 0.0)).xyz);
	toLightVector = normalize(lightpos.xyz - worldPos.xyz);
	passTexCoords = texCoords;
	gl_Position = projection * view * worldPos;
}