
attribute vec2 a_Position;
attribute vec4 a_Color;
attribute vec2 a_TexCoords;

varying vec4 v_Color;
varying vec2 v_TexCoords;

uniform mat4 worldMatrix;
uniform mat4 projMatrix;
uniform mat4 viewMatrix;

void main(void)
{
	v_Color = abs(gl_Position);
	v_TexCoords = a_TexCoords;

	mat2 scaleMatrix = mat2(
							100.0, 0.0,
							0.0, 100.0
						);

	float width = 1280.0;
	float height = 720.0;
	vec2 newPosition =   a_Position.xy;		//projectionMatrix * worldMatrix * a_Position.xy;

	gl_Position = vec4( newPosition , 0.0, 1.0 );
}