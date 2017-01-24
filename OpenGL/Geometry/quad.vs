attribute vec2 a_Position;
//attribute vec2 a_TexCoords;

varying vec2 v_TexCoords;

void main(void)
{
	vec4 v_Position = vec4(a_Position.xy, 0.0, 1.0);
	// converti des coordonnees exprimees entre [-1;+1] vers [0;1]
	v_TexCoords = a_Position * 0.5 + 0.5;

	gl_Position = v_Position;
}