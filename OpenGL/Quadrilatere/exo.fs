uniform sampler2D tex;
 
 //const vec3 lum = vec3();

void main(void)
{
    gl_FragColor = texture2D(tex,gl_TexCoord[0].st);
}