#define TYPE solid

varying float depth;

void main() {
	vec4 vInfo = gl_TexCoord[0];
	float depth = vInfo.z / vInfo.x;
	gl_FragColor = vec4(depth, depth * depth, 0.0, 1.0);
}