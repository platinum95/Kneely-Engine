#version 330 core

layout (std140) uniform camera_data
{ 
	mat4 projection;
	mat4 view;
};


in vec3 init_velocity;
in float init_time;
in float init_size;
in vec3 init_colour;
in float init_opacity;
in float lifetime;
/*in float shape;*/

out float pass_time;
out vec3 pass_colour;
out float pass_opacity;



uniform float time;
uniform vec4 emitter_position;
uniform vec3 base_direction;
uniform mat4 transformationMatrix;

void main(){
	pass_colour = init_colour;
	float elapsed_time = time - init_time;
	elapsed_time = mod (elapsed_time, 5.0);
	pass_opacity = 1.0;
	float lifetime_decay = lifetime / 3.5;
	if(elapsed_time > lifetime_decay){
		float decay_time = lifetime - elapsed_time;
		pass_opacity = max(decay_time/lifetime_decay, 0);
	}
	pass_time = elapsed_time;
	vec3 ePos = vec3(0, 0, 0);//vec3(emitter_position.xyz);
	vec3 a = vec3(0, -1, 1);
	vec3 velocity = init_velocity + base_direction;
	ePos += velocity * elapsed_time + 0.5 * a * elapsed_time * elapsed_time;
	gl_Position = projection * view * transformationMatrix * vec4(ePos, 1);
	gl_PointSize = init_size;

}






