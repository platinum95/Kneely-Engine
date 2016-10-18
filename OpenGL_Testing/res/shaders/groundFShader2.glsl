
#version 330 core

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;  

uniform sampler2D tex, normalTex;
in vec2 texCoords;
in vec4 vertPos;
in float fade;
in float distanceFrom;
in mat3 TBN;
in vec3 LightDirection_tangentspace;
in vec3 EyeDirection_tangentspace;
in vec3 norm, tang, bitang, view_dir_tan, light_dir_tan;

flat in int clipp;

layout (std140) uniform light{
	vec4 lightpos;
	vec4 lightColour;
};

vec3 getNormal(vec2 _coord);

void main(){

	vec3 unitNormal = normalize(texture(normalTex, texCoords ).rgb * 2.0 - 1.0);//normalize(  texture(normalTex, texCoords ).rgb*2.0 - 1.0).rgb;
	float cosTheta = clamp( dot( unitNormal, LightDirection_tangentspace ), 0,1 );
	vec3 direction_to_light_tan = normalize(-light_dir_tan);
	float dot_prod = max(dot(direction_to_light_tan, unitNormal), 0);
	vec3 Id = vec3(0.7, 0.7, 0.7) * vec3(1.0, 0.5, 0.0) * dot_prod;

	vec3 R = reflect(-LightDirection_tangentspace,unitNormal);
	float cosAlpha = clamp( dot( EyeDirection_tangentspace,R ), 0,1 );

	vec4 MaterialColour = texture(tex, texCoords) * 0.4;
	vec3 MaterialAmbientColor = vec3(0.3,0.4,0.3) * MaterialColour.xyz;
	vec4 outputColour = vec4(MaterialAmbientColor +
		 MaterialColour.xyz * lightColour.xyz * 1 * cosTheta +
		  clamp(vec3(1, 1, 1) * lightColour.xyz * 1 * pow(cosAlpha,5) / (distanceFrom), 0, 0.3), 1.0);

	vec3 lightDir = normalize(vec4(0, 10, -20, 1) - vertPos).xyz;
	float diff = max(dot(lightDir, unitNormal), 0.0);
	vec4 diffuse2 = diff * MaterialColour;

	if(clipp == 1){
		FragColor = vec4(0, 0, 0, 1);
	}
	else{
		FragColor = vec4(Id, 1.0) + MaterialColour;//outputColour;//vec4(unitNormal, 1.0);//outputColour;
	}

	

	float brightness2 = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
    
	if(brightness2 > 0.6)
;//        BrightColor = vec4(FragColor.rgb, 1.0);
	else
		BrightColor = vec4(0, 0, 0, 1);
	
}

