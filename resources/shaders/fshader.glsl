#version 330 core

in vec2 v_texcoord;
in vec3 v_normal;
in vec3 v_light;
in vec3 v_tangent;
in vec3 v_bitangent;

out vec4 color;

uniform sampler2D colorTexture;
uniform sampler2D normalTexture;

const float ambientStrength = 0.2f; // TODO: fix
const float diffuseStrength = 1.0f; // TODO: fix

const vec4 lightColor = vec4(1.0, 1.0, 1.0, 1.0);

void main()
{
    vec3 texNormal = texture(normalTexture, v_texcoord).xyz;
    texNormal = texNormal * 2.0 - 1.0;

    vec3 T = normalize(v_tangent);
    vec3 B = normalize(v_bitangent);
    vec3 N = normalize(v_normal);
    mat3 TBN = mat3(T, B, N);

    vec3 n = normalize(TBN * texNormal);
    vec3 l = normalize(v_light);

    float diff = max(dot(n, l), 0.0);

    vec4 texColor = texture(colorTexture, v_texcoord);
    vec4 ambient = texColor * ambientStrength;
    vec4 diffuse = texColor * lightColor * diff * diffuseStrength;

    color = ambient + diffuse;
}