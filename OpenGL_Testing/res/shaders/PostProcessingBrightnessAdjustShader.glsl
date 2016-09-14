
void BrightnessEffect(){
	outFrag = max(outFrag + BrightnessAmount, vec4(0.0, 0, 0, 0));
	outFrag = min(outFrag, vec4(1.0, 1, 1, 1));
}