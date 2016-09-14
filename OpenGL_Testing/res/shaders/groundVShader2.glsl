
#version 330

in vec2 position;
in float height;
in vec2 textureCoords;
in vec3 normal;

out vec2 texCoords;
out vec4 vertPos;
out vec3 toLightVector;
out vec3 surfaceNormal;
out vec3 lightc;
out float fade;
flat out int clipp;
out float distanceFrom;

layout (std140) uniform camera_data
{ 
	mat4 projection;
	mat4 view;
};

layout (std140) uniform clip_plane{
	vec4 plane;
};

layout (std140) uniform light{
	vec4 lightpos;
	vec4 lightColour;
};



uniform mat4 translation;
uniform bool clippy;
uniform vec4 clipPlane;

void main(){
	vec4 currentPos = vec4(position.x, height, position.y, 1);
	vec4 worldPos = translation * currentPos;
	distanceFrom = distance(lightpos, worldPos);
	texCoords = textureCoords;
	gl_ClipDistance[0] = dot(worldPos, plane);
	lightc = lightColour.xyz;
	fade = max(1.0f - distanceFrom/100.0f, 0);
	
	if(clippy && (dot(worldPos, clipPlane) < -1 || clipPlane.w == 0)){
		clipp = 1;
	}
	else{
		clipp = 0;
	}

	vertPos = currentPos;
	vec4 relPos = projection * view * worldPos;
	surfaceNormal = normalize((translation * vec4(normal, 0.0)).xyz);
	toLightVector = normalize(lightpos.xyz - worldPos.xyz);
	
	gl_Position = relPos;
}

