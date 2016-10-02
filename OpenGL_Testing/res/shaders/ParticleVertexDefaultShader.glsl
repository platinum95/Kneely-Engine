#version 330 core

layout (std140) uniform camera_data
{ 
	mat4 projection;
	mat4 view;
};

layout (location = 1) in float startTime;

in vec3 init_velocity;
out float pass_time;
/*
in float init_size;
in vec3 init_colour;
in float init_opacity;
in float shape;
in float lifetime;*/uniform float time;uniform vec4 emitter_position;void main(){	float elapsed_time = time - startTime;	elapsed_time = mod (elapsed_time, 5.0);	pass_time = elapsed_time;	vec3 ePos = vec3(0, 10, 0);
	vec3 a = vec3(0, -1, 0);
	ePos += init_velocity * elapsed_time + 0.5 * a * elapsed_time * elapsed_time;
	gl_Position = projection * view * vec4(ePos, 1);
	gl_PointSize = 1.0;}