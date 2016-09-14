
#version 330 core

in vec2 position;
in vec2 texCoords;

out vec2 passTexCoords;


void main(){
	passTexCoords = texCoords;	
	gl_Position = vec4(position, 0, 1);
}