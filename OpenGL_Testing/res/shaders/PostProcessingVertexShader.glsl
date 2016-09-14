
#version 330


in vec2 position;
in vec2 inTexCoord;

out vec2 texCoord;

void main(){
	gl_Position = vec4(position, 0, 1);
	texCoord = inTexCoord;
}