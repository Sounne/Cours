attribute vec4 a_Position;
attribute vec3 a_Normal;
attribute vec2 a_TexCoords;

varying vec4 v_Position;
varying vec3 v_Normal;
varying vec2 v_TexCoords;
varying mat4 modelMatrix;

uniform mat4 worldMatrix;
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

void main(void)
{
	v_TexCoords = a_TexCoords;
	v_Normal = a_Normal;

	modelMatrix = viewMatrix * worldMatrix;

	gl_Position = projectionMatrix * modelMatrix * a_Position;
}