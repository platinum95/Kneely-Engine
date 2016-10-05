#version 330 core

in vec3 position;
in vec3 normal;

out vec3 toLightVector;
out vec3 surfaceNormal;
out float pass_time;
out vec3 pass_colour;

uniform mat4 translation;
uniform float time;

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

float radToPi(float rad){
	return rad * 57.6;
}

void main(){
	float zPos = position.z;
	pass_time = time;
	float timey = mod(time, 6.28);
	float sinBase = 0.03 * zPos + timey*0.02;
	float beta = 0.3;
	pass_colour = vec3(sin(radToPi(sinBase + 0)) + beta, sin(radToPi(sinBase + 2)) + beta  , sin(radToPi(sinBase + 4))) + beta;
	float sinVal = position.z - timey;
	float offset = 0.7 * sin(sinVal);
	float offsetC = 2.0 * cos(sinVal);
	vec3 offsetPos = position;
	offsetPos.y += offset;
	offsetPos.x += offsetC;

	vec4 worldPos = translation * vec4(offsetPos, 1);
	gl_ClipDistance[0] = dot(worldPos, plane);
	surfaceNormal = normalize((translation * vec4(normal, 0.0)).xyz);
	toLightVector = normalize(lightpos.xyz - worldPos.xyz);

	gl_Position = projection * view * worldPos;
}
