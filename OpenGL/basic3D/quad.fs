varying vec2 v_TexCoords;

uniform sampler2D u_Texture;

void main(void)
{
    vec4 texColor = texture2D(u_Texture, v_TexCoords);
    gl_FragColor = texColor;
}