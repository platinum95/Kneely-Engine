

void SaturationEffect() {


	float Pr = 0.299;
	float Pg = 0.587;
	float Pb = 0.114;
	float R = outFrag.r;
	float G = outFrag.g;
	float B = outFrag.b;
	float P = sqrt((R)*(R)*Pr + (G)*(G)*Pg + (B)*(B)*Pb ) ;

	R=P+((R)-P)*SaturationAmount;
	G=P+((G)-P)*SaturationAmount;
	B=P+((B)-P)*SaturationAmount; 
	outFrag = vec4(R, G, B, outFrag.a);
	}
	