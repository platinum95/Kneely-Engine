
#version 330 core

in vec4 colourOffset;
out vec4 FragColor;


void main(){

	//if(colourOffset.a > 0)
		FragColor = vec4(colourOffset.r, colourOffset.g, colourOffset.b, 1);
	//else
		//FragColor = vec4(1, 0, 0, 1);
	
}
