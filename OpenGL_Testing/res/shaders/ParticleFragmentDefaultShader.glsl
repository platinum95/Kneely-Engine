#version 330 core

layout (location = 0) out vec4 frag_colour;
layout (location = 1) out vec4 BrightColor;

in float pass_time;
in vec3 pass_colour;
in float pass_opacity;

void main(){
	frag_colour = vec4(pass_colour, pass_opacity);
	BrightColor = pass_opacity * vec4(pass_colour, pass_opacity);
}
