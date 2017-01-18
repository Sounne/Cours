varying vec2 v_TexCoords;

uniform sampler2D u_texture;

//const vec3 luma = vec3(0.20, 0.56, 0.80);

void main(void)
{
    vec4 texColor = texture2D(u_texture, v_TexCoords);

    gl_FragColor = texColor;
    //gl_FragColor = dot(texColor, luma);
}