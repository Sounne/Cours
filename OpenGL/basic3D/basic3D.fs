
// INPUTS du Fragment Shader = OUTPUTS du Vertex Shader

varying vec4 v_Position;
varying vec2 v_TexCoords;
varying vec3 v_Normal;

varying mat4 modelMatrix;

uniform sampler2D u_texture;
uniform float u_power;
uniform vec3 u_light_pos;
uniform vec3 u_cam_pos;

struct Light
{
    vec3 position;
    vec3 colorD;
    vec3 colorS;
    float power;
    float shininess;
} light;

void main(void)
{
    //light initialization
    light.position = vec3(0.0, -1.0, 0.0);
    light.colorD = vec3(1.0, 0.0, 0.0);
    light.colorS = vec3(1.0, 1.0, 1.0);
    light.power = u_power;
    light.shininess = 20.0;

    //texture
    vec2 texCoords = vec2(v_TexCoords.x, 1.0 - v_TexCoords.y);
    vec4 texColor = texture2D(u_texture, texCoords);

    //diffuse
    vec3 normal = normalize(v_Normal);

    vec3 fpos = vec3(modelMatrix * v_Position);
    vec3 direction = light.position - fpos;

    float theta = dot(normal, direction);
    float I = max(cos(theta), 0);

    vec3 diffuse_color = I * light.power * light.colorD;

    //specular
    vec3 V = normalize(u_cam_pos - v_Position.xyz);
    vec3 R = reflect(direction, normal);

    //vec3 R = normalize((direction+V)/2);

    float angle = dot(V, R);

    vec3 specular_color = light.colorS * pow(max(angle, 0), light.shininess);

    gl_FragColor = vec4(diffuse_color * texColor.rgb + specular_color, texColor.a);
}
