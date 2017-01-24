#version 330 core

out VertexOut
{
	vec4 v_Position;
	vec3 v_Normal;
	vec2 v_TexCoords;
} OUT;

in vec4 a_Position; // x,y,z,?
in vec3 a_Normal;
in vec2 a_TexCoords;

out vec4 v_Position;
out vec3 v_Normal;
out vec2 v_TexCoords;

uniform mat4 worldMatrix;
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

out mat4 modelMatrix;

void main(void)
{
	//gl_Position = gl_in[0].gl_Position + vec4(-0.1, 0.0, 0.0, 0.0);
	//EmitVertex();

	v_TexCoords = a_TexCoords;
	v_Normal = a_Normal;

	modelMatrix = viewMatrix * worldMatrix;

	gl_Position = projectionMatrix * modelMatrix * a_Position;
}
