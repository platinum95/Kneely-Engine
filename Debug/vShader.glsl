

#version 330

in vec3 position;
uniform float time;

out offset;


int main(){
	gl_Position = position;
	float posVal = sin(time);
	offset = (127) * (posVal);

}