
// INPUTS du Vertex Shader

// note: lorsqu'il n'y a qu'un seul 'attribute'
// on sait qu'il aura le 'location' 0

// le GPU rempli automatiquement les valeurs manquantes
// d'un attribute par 0.0 pour x,y,z et 1.0 pour w

attribute vec4 a_Position; // x,y,z,?

varying vec4 v_Color;

uniform mat4 worldMatrix;
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

void main(void)
{
	v_Color = abs(a_Position);

	gl_Position = projectionMatrix * viewMatrix * worldMatrix * a_Position;
}
