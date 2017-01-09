
// INPUTS du Fragment Shader = OUTPUTS du vertex shader

varying vec4 v_Color;

void main(void)
{
    //variable predefinies OBLIGATOIRE en OUTPUT vec4 gl_FragColor

    gl_FragColor = v_Color;
    //gl_FragColor = vec4(1.0, 0.0, 0.0, 0.0);
}
