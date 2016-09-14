
#version 330 core

in vec3 position;
varying vec2 vertPos;
out vec2 texCoord;

out vec4 clipSpace;
out vec3 cameraPosition;
out vec4 passLightColour;
out vec3 fromLightPos;
out vec3 fromCameraPos;

uniform float waveSet;

layout (std140) uniform camera_data
{ 
	mat4 projection;
	mat4 view;
	vec4 camera_position;
};

layout (std140) uniform light{
	vec4 lightpos;
	vec4 lightColour;
};


vec3 getCameraPos(mat4 viewMat){
	mat3 rotMat = mat3 (viewMat);
	vec3 d = vec3(viewMat[3]);
	return -d * rotMat;
}

void main(){
	passLightColour = lightColour;
	vec4 worldPos = vec4(position, 1);

	vertPos = position.xz;
	cameraPosition = getCameraPos(view);//camera_position.xyz;
	fromLightPos = worldPos.xyz - lightpos.xyz;
	fromCameraPos = cameraPosition - worldPos.xyz;

	clipSpace = projection * view * worldPos;
	texCoord = vec2(position.x/2.0 + 0.5, position.z/2.0 + 0.5) * 0.2;
	gl_Position = clipSpace;

	//

	
}
