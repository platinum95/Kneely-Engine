
#version 330

in int offset;
out vec4 fragColour;


int main(){
/*
	if(offset == 0){
		fragColour = vec4(0, 0, 0, 1);
	}
	else{
		int realOffset = offset + 127;

		fragColour = vec4(124, realOffset, 40, 1);
	}
	*/

	fragColour = (120, 140, 120, 1);

}