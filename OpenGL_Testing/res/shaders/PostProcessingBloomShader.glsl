
uniform float weight[5] = float[] (0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);

vec4 blur(sampler2D image, vec2 uv, vec2 resolution, vec2 direction) {
  vec4 color = vec4(0.0);
  vec2 off1 = vec2(1.411764705882353) * direction;
  vec2 off2 = vec2(3.2941176470588234) * direction;
  vec2 off3 = vec2(5.176470588235294) * direction;
  color += texture2D(image, uv) * 0.1964825501511404;
  color += texture2D(image, uv + (off1 / resolution)) * 0.2969069646728344;
  color += texture2D(image, uv - (off1 / resolution)) * 0.2969069646728344;
  color += texture2D(image, uv + (off2 / resolution)) * 0.09447039785044732;
  color += texture2D(image, uv - (off2 / resolution)) * 0.09447039785044732;
  color += texture2D(image, uv + (off3 / resolution)) * 0.010381362401148057;
  color += texture2D(image, uv - (off3 / resolution)) * 0.010381362401148057;
  return color;
}

void BloomEffect(){
	
	float brightness = (outFrag.r * 0.2) + (outFrag.g * 0.7) + (outFrag.b * 0.7);
	
	if(brightness < 0.9){
	;//	return;
	} 

	vec2 iResolution = resolution;
	vec2 uv = vec2(gl_FragCoord.xy / iResolution.xy);
	vec4 out_colour = blur(colour_attachment_1, uv, iResolution.xy, vec2(0, 1));
	out_colour += 0.5 * blur(colour_attachment_1, uv, iResolution.xy, vec2(1, 0));
	
	
	
	/*
	vec4 out_colour = vec4(0.0);
	out_colour += texture(colour_attachment_0, texCoord - vec2(0,5)) * 0.0093;
    out_colour += texture(colour_attachment_0, texCoord - vec2(0,4)) * 0.028002;
    out_colour += texture(colour_attachment_0, texCoord - vec2(0,3)) * 0.065984;
    out_colour += texture(colour_attachment_0, texCoord - vec2(0,2)) * 0.121703;
    out_colour += texture(colour_attachment_0, texCoord - vec2(0,1)) * 0.175713;
    out_colour += texture(colour_attachment_0, texCoord ) * 0.198596;
    out_colour += texture(colour_attachment_0, texCoord + vec2(0,1)) * 0.175713;
    out_colour += texture(colour_attachment_0, texCoord + vec2(0,2)) * 0.121703;
    out_colour += texture(colour_attachment_0, texCoord + vec2(0,3)) * 0.065984;
    out_colour += texture(colour_attachment_0, texCoord + vec2(0,4)) * 0.028002;
    out_colour += texture(colour_attachment_0, texCoord + vec2(0,5)) * 0.0093;
	
	
	out_colour += texture(colour_attachment_0, texCoord - vec2(5,0)) * 0.0093;
    out_colour += texture(colour_attachment_0, texCoord - vec2(4,0)) * 0.028002;
    out_colour += texture(colour_attachment_0, texCoord - vec2(3,0)) * 0.065984;
    out_colour += texture(colour_attachment_0, texCoord - vec2(2,0)) * 0.121703;
    out_colour += texture(colour_attachment_0, texCoord - vec2(1,0)) * 0.175713;
    out_colour += texture(colour_attachment_0, texCoord ) * 0.198596;
    out_colour += texture(colour_attachment_0, texCoord + vec2(1,0)) * 0.175713;
    out_colour += texture(colour_attachment_0, texCoord + vec2(2,0)) * 0.121703;
    out_colour += texture(colour_attachment_0, texCoord + vec2(3,0)) * 0.065984;
    out_colour += texture(colour_attachment_0, texCoord + vec2(4,0)) * 0.028002;
    out_colour += texture(colour_attachment_0, texCoord + vec2(5,0)) * 0.0093;
	*/
	
	vec4 result = out_colour;
	
	/*
	bool horizontal = true;
	vec2 tex_offset = 1.0 / textureSize(colour_attachment_0, 0); // gets size of single texel
    vec3 result = texture(colour_attachment_0, texCoord).rgb * weight[0]; // current fragment's contribution
    if(horizontal)
    {
        for(int i = 1; i < 5; ++i)
        {
            result += texture(colour_attachment_0, texCoord + vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
            result += texture(colour_attachment_0, texCoord - vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
        }
    }
    else
    {
        for(int i = 1; i < 5; ++i)
        {
            result += texture(colour_attachment_0, texCoord + vec2(0.0, tex_offset.y * i)).rgb * weight[i];
            result += texture(colour_attachment_0, texCoord - vec2(0.0, tex_offset.y * i)).rgb * weight[i];
        }
    }
    vec4 resultColour = vec4(result, 1.0);
	*/
	if(length(out_colour) > 0.01)
		outFrag += out_colour;
	
	//else
		outFrag = outFrag;
		
}