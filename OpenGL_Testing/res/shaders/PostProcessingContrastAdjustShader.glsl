
void ContrastEffect(){
	outFrag = max(vec4(0, 0, 0, 0), outFrag * ContrastAmount);
	outFrag = min(vec4(1.0, 1.0, 1.0, 1.0), outFrag);
	//outFrag = vec4(0, 1, 0, 1);
}