void main() {
	vec4 tPos = xi_modelViewProjectionMatrix * gl_Vertex;
	gl_Position = tPos;
	gl_TexCoord[0] = vec4( xi_depthFar * 0.5, tPos.y, tPos.z, tPos.w );

	gl_TexCoord[1].xy = gl_MultiTexCoord0.xy;
}