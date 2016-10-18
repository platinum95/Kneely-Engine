
#version 330

in vec2 position;
in float height;
in vec2 textureCoords;
in vec3 normal;
in vec3 tangent;
in vec3 bitangent;

out vec2 texCoords;
out vec4 vertPos;
out float fade;
out mat3 TBN;
out vec3 LightDirection_tangentspace;
out vec3 EyeDirection_tangentspace;
out vec3 norm, tang, bitang, view_dir_tan, light_dir_tan;

flat out int clipp;
out float distanceFrom;

layout (std140) uniform camera_data
{ 
	mat4 projection;
	mat4 view;
};

layout (std140) uniform clip_plane{
	vec4 plane;
};

layout (std140) uniform light{
	vec4 lightpos;
	vec4 lightColour;
};



uniform mat4 translation;
uniform bool clippy;
uniform vec4 clipPlane;

void main(){
	vec4 currentPos = vec4(position.x, height, position.y, 1);
	vec4 Position_worldspace  = translation * currentPos;
	vec4 vertexPosition_cameraspace = view * Position_worldspace;
	vec3 EyeDirection_cameraspace = vec3(0,0,0) - vertexPosition_cameraspace.xyz;
	vec3 LightPosition_cameraspace = ( view * vec4(lightpos.xyz,1)).xyz;
	vec3 LightDirection_cameraspace = LightPosition_cameraspace + EyeDirection_cameraspace;
	vec3 Normal_cameraspace = ( view * translation * vec4(normal,0)).xyz;

	vec3 cam_pos_loc = vec3(inverse(translation) * lightpos);
	vec3 light_dir_wor = vec3(0, 0, -1);
	vec3 light_dir_loc = vec3(inverse(translation) * vec4(light_dir_wor, 0));
	vec3 view_dir_loc = normalize(cam_pos_loc - currentPos.xyz);
	view_dir_tan = vec3(dot(tangent.xyz, view_dir_loc),
						dot(bitangent.xyz, view_dir_loc),
						dot(normal.xyz, view_dir_loc)
						);
	light_dir_tan = vec3(dot(tangent.xyz, light_dir_loc),
						dot(bitangent.xyz, light_dir_loc),
						dot(normal.xyz, light_dir_loc)
						);

	distanceFrom = distance(lightpos, Position_worldspace);
	texCoords = textureCoords;
	gl_ClipDistance[0] = dot(Position_worldspace, plane);
	fade = max(1.0f - distanceFrom/100.0f, 0);
	
	if(clippy && (dot(Position_worldspace, clipPlane) < -1 || clipPlane.w == 0)){
		clipp = 1;
	}
	else{
		clipp = 0;
	}
	
	gl_Position = projection * vertexPosition_cameraspace;
	TBN = transpose(mat3(
		tangent,
		bitangent,
		normal
	));
	LightDirection_tangentspace = normalize(TBN * LightDirection_cameraspace);
	EyeDirection_tangentspace =  normalize(TBN * EyeDirection_cameraspace); 
	norm = normal;
	tang = tangent;
	bitang = bitangent;
}

