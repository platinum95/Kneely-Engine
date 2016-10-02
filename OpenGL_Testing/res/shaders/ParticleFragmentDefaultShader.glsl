#version 330 core

layout (location = 0) out vec4 frag_colour;

layout (location = 1) out vec4 BrightColor;
in float pass_time;
void main(){
	frag_colour = vec4(1, 0.5498, 0.27, 1);
	BrightColor = vec4(1, 0.5498, 0.27, 1);
}
