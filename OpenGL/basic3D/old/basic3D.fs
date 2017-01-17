
// INPUTS du Fragment Shader = OUTPUTS du Vertex Shader

uniform vec2 lum;
uniform sampler2DRect tex0;

varying vec4 v_Color;
varying vec2 texcoord0;


void main(void)
{
    // variable predefinies OBLIGATOIRE en OUTPUT
    // vec4 gl_FragColor

    v_Color = vec4(0.5, 0.5, 0.5, 1.0);
    
    vec4 a = texture2DRect(tex0, texcoord0);
    const vec4 coef = vec4(0.299,0.587,0.114,0.);
    float luminance = dot(v_Color, coef);

    float clo = step(lum.x, luminance);
    float chi = step(luminance, lum.y);
    float amask = clo * chi;

    gl_FragColor = vec4(v_Color.rgb, amask);

//    gl_FragColor = v_Color;
}
