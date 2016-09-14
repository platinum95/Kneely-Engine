
#version 330 core

in vec2 passTexCoords;

uniform sampler2D tex;

out vec4 color;

void main(){
	color = texture(tex, passTexCoords);
	//color = vec4(0,1,0,1);
}
