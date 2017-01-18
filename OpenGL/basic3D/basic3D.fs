varying vec4 v_Position;
varying vec2 v_TexCoords;
varying vec3 v_Normal;
varying mat4 modelMatrix;

uniform sampler u_Texture;
uniform float u_power;

struct Light
{
    vec3 position;
    vec3 color;
    float power;
}   light;

void main(void)
{
    light.position = vec3(0.0, 1.0, 0.0);
    light.color = vec3(1.0, 1.0, 1.0);
    
    vec2 texCoords = vec2(v_TexCoords.x, 1.0 - v_TexCoords.y);
    vec4 texColor = texture2D(u_Texture, texCoords);
    vec3 normal = normalize(v_Normal);

    vec3 fpos = vec3(modelMatrix * v_Position);
    vec3 direction = light.position * fpos;

    float theta = dot(normal, direction) / (length(direction) * length(normal));
    
    gl_FragColor = vec4(light.color * texColor.rgb, texColor.a);
}
