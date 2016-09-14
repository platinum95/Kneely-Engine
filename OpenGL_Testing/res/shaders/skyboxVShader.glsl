#version 330 

in vec3 position;
out vec3 textureCoords;

layout (std140) uniform camera_data
{ 
	mat4 projection;
	mat4 view;
};

layout (std140) uniform clip_plane{
	vec4 plane;
};


void main(void){
	vec4 currentPos = vec4(position, 1.0);
	mat4 view2 = view;
	view2[3][0] = 0;
	view2[3][1] = 0;
	view2[3][2]= 0;
	textureCoords = position;
	gl_ClipDistance[0] = dot(currentPos, plane);
	gl_Position = projection * view2 * currentPos; 
	
	
}
