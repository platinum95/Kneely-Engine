
#version 330 core



layout (location = 0) out vec4 outCol;
layout (location = 1) out vec4 BrightColor;  

in vec4 clipSpace;
in vec2 texCoord;
in vec3 cameraPosition;
in vec4 passLightColour;
in vec3 fromCameraPos;
in vec3 fromLightPos;
varying vec2 vertPos;

const float shineDamper = 20.0f;
const float reflectivity = 0.6f;
const float waveStrength = 0.04f;
const float maxWaveDistance = 50;

uniform float waveSet;
uniform sampler2D reflectionTex, refractionTex, refractionDepth, DUDVMap, normalMap;

layout (std140) uniform light{
	vec4 lightpos;
	vec4 lightColour;
};

float getDistance(vec3 _cameraPosition, vec2 _vertexPosition){
	return max(distance(_cameraPosition.xz, _vertexPosition), 0.01);
}

float getFresnelMix(vec3 _cameraPosition, float distance){
	float angle = atan(abs(_cameraPosition.y)/distance);
	return clamp(angle / (1.5/2.0), 0.1, .9);
}

float getDepth(vec2 refractionCoord){
	float nearPlane = .1, farPlane = 1000;
	float depth = texture(refractionDepth, refractionCoord).r; 
	float floorDistance = 2.0 * nearPlane * farPlane / (farPlane + nearPlane - (2.0 * depth - 1.0) * (farPlane - nearPlane));
	depth = gl_FragCoord.z;
	float waterDistance = 2.0 * nearPlane * farPlane / (farPlane + nearPlane - (2.0 * depth - 1.0) * (farPlane - nearPlane));
	return floorDistance - waterDistance;
}

vec2 getDistortion(){
	//vec2 distortedTexCoords = texture(DUDVMap, vec2(texCoord.x, texCoord.y)).rg*0.1;
	//distortedTexCoords = texCoord + vec2(distortedTexCoords.x, distortedTexCoords.y);
	//return (texture(DUDVMap, distortedTexCoords).rg * 2.0 - 1.0) * 0.09;
	return 0.09 * (texture(DUDVMap, vec2(texCoord.x, texCoord.y)).rg * 2.0 - 1.0);
}

vec3 getNormal(vec2 _coord){
	vec3 normal = vec3(texture(normalMap, _coord));
	normal.r = normal.r * 2.0 - 1.0;
	normal.g = normal.g * 2.0 - 1.0;
	normal.b = normal.b * 2.0 - 1.0;
	return normalize(normal);
}

vec3 getSpecularHighlights(vec3 _lightVector, vec3 _viewVector, vec3 _normal){
	vec3 reflectedLight = reflect(normalize(_lightVector), _normal);
	float specular = max(dot(reflectedLight, _viewVector), 0.0);
	specular = pow(specular, shineDamper);
	return passLightColour.rgb * specular * reflectivity;
}

void main(){
	float distance = getDistance(cameraPosition, vertPos);

	//vec2 distortion = getDistortion();
	float totalWaveStrength = waveStrength * (1.0 - clamp(distance/maxWaveDistance, 0.0, 0.99));
	vec2 distortedTexCoords = texture(DUDVMap, vec2(texCoord.x + waveSet, texCoord.y)).rg*0.1;
	distortedTexCoords = texCoord + vec2(distortedTexCoords.x, distortedTexCoords.y+waveSet);
	vec2 distortion = (texture(DUDVMap, distortedTexCoords).rg * 2.0 - 1.0) * totalWaveStrength;

	

	vec3 normal = getNormal(distortedTexCoords);

	vec3 viewVector = normalize(fromCameraPos);

	vec3 specularHighlights = getSpecularHighlights(fromLightPos, viewVector, normal);

	vec3 ndc = (clipSpace.xyz/clipSpace.w)/2.0 + 0.5;
	vec2 reflectCoords = vec2(ndc.x, -ndc.y);
	vec2 refractCoords = vec2(ndc.x, ndc.y);

	float waterDepth = getDepth(refractCoords);

	vec2 waveStrength = distortion * clamp(waterDepth/3.0, 0.0, 1.0);

	vec4 reflectColour = texture(reflectionTex, reflectCoords + waveStrength);
	vec4 refractColour = texture(refractionTex, refractCoords + waveStrength);

//	vec4 reflectColour = texture(reflectionTex, reflectCoords);
//	vec4 refractColour = texture(refractionTex, refractCoords);

	float mixNum = getFresnelMix(cameraPosition, distance);

	outCol = mix(reflectColour, refractColour, mixNum) + vec4(specularHighlights, 0.0);
	BrightColor = vec4(0, 0, 0, 1);
	float brightness = dot(outCol.rgb, vec3(0.2126, 0.7152, 0.0722));
    

	outCol.a =  clamp(waterDepth/2.0, 0.0, 1.0);
	if(brightness > 0.8)
        BrightColor = vec4(outCol.rgb, 1.0);
	else
		BrightColor = vec4(0, 0, 0, 1);
	
}
