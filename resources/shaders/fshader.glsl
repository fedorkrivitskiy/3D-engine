#version 330 core

in vec2 v_texcoord;
in vec3 v_normal;
in vec3 v_light;

out vec4 color;

uniform sampler2D colorTexture;
// uniform sampler2D normalTexture;

const float ambientStrength = 0.2f;
const float diffuseStrength = 1.0f;

const vec4 lightColor = vec4(1.0, 1.0, 1.0, 1.0);

void main()
{
    vec3 n = normalize(v_normal);
    vec3 l = normalize(v_light);

    float diff = max(dot(n, l), 0.0);

    vec4 texColor = texture(colorTexture, v_texcoord);
    vec4 ambient = texColor * ambientStrength;
    vec4 diffuse = texColor * lightColor * diff * diffuseStrength;

    color = ambient + diffuse;
}