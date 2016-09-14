
#version 330 core

in vec3 position;
in vec2 textureCoords;
in vec3 normal;

out vec2 texCoords;
out vec4 vertPos;
out vec3 toLightVector;
out vec3 surfaceNormal;
flat out int clipp;

layout (std140) uniform camera_data
{ 
	mat4 projection;
	mat4 view;
};

layout (std140) uniform light{
	vec3 lightpos;
	vec3 lightColour;
};

uniform mat4 translation;
uniform bool clippy;
uniform vec4 clipPlane;

void main(){
	vec4 currentPos = vec4(position, 1);
	vec4 worldPos = translation * currentPos;
	texCoords = textureCoords;
	gl_ClipDistance[0] = 1;
	
	if(clippy&& (dot(worldPos, clipPlane) < -1 || clipPlane.w == 0)){
		clipp = 1;
		
	}
	else{
		clipp = 0;
	}
	vertPos = currentPos;
	vec4 relPos = projection * view * worldPos;
	surfaceNormal = (translation * vec4(normal, 0.0)).xyz;
	toLightVector = lightpos - worldPos.xyz;
	
	gl_Position = relPos;
}
