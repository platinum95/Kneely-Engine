 
#version 330 core

in vec3 position;
in vec3 normal;
in float distance;

out vec4 colourOffset;

uniform float time;
uniform float alpha;
uniform float timeDiff[3];
uniform bool clippy;


layout (std140) uniform waveform{
	float heartbeat[1544];
	vec3 heartcolour[1544];
};

layout (std140) uniform camera_data
{ 
	mat4 projection;
	mat4 view;
};

layout (std140) uniform clip_plane{
	vec4 plane;
};


void main(){
	
	vec4 currentPos = vec4(position, 1);
	float MAGNITUDE = 0;
	float length = 10.0f;
	float scalify = length / 1544;
	bool wasColour = false;
	gl_ClipDistance[0] = dot(currentPos, plane);
	if(timeDiff[0] > 0){
		float startingPoint = timeDiff[0] * 40;
		float endingPoint = startingPoint - length;
		
		
		if(distance < startingPoint && distance > endingPoint){
			float dr = distance - endingPoint;
			float findex = (dr / scalify);
			highp int index = int(findex);
			float wf = heartbeat[index];
			MAGNITUDE = wf * 20.0f * alpha;
			colourOffset = vec4(heartcolour[index] * alpha, alpha);
		
			wasColour = true;
		}
		if(clippy){
			float radi = endingPoint/50.0f;
			float disti = 50.0f - (cos(radi) * 50.0f);
			vec4 plane = vec4(0, 0, 1, 50 - disti);
			gl_ClipDistance[0] = dot(currentPos, plane);
		}
		else
			gl_ClipDistance[0] = 1;
		
	}
	else{
		gl_ClipDistance[0] = 1;
	}
	
	
	
	if(!wasColour)
		colourOffset = vec4(0, 0, 0, 0);

	/*
	float powerthing = pow(distance, time);
	zOff = ((1 + cos(powerthing))/2);
	if(distance == 0){
		zOff = 0;
	}
	*/
	

	vec4 finalPos = currentPos + (vec4(normal, 0.0f) * MAGNITUDE);
	//vec4 finalPos = projection * view * currentPos;
	//gl_Position = finalPos + vec4(thisNormal, 0.0f) * MAGNITUDE;
	gl_Position = projection * view * finalPos;
}


