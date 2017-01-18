
// INPUTS du Vertex Shader

// note: lorsqu'il n'y a qu'un seul 'attribute'
// on sait qu'il aura le 'location' 0

// le GPU rempli automatiquement les valeurs manquantes
// d'un attribute par 0.0 pour x,y,z et 1.0 pour w

attribute vec4 a_Position; // x,y,z,?
attribute vec3 a_Normal;
attribute vec2 a_TexCoords;

varying vec4 v_Position;
varying vec3 v_Normal;
varying vec2 v_TexCoords;

uniform mat4 worldMatrix;
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

varying mat4 modelMatrix;

void main(void)
{
	v_TexCoords = a_TexCoords;
	v_Normal = a_Normal;

	modelMatrix = viewMatrix * worldMatrix;

	gl_Position = projectionMatrix * modelMatrix * a_Position;
}
