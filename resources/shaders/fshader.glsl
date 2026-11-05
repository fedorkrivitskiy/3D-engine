#version 330 core

in vec2 v_texcoord;
in vec3 v_normal;
in vec3 v_tangent;
in vec3 v_bitangent;
in vec3 v_worldPos;

out vec4 color;

uniform sampler2D colorTexture;
uniform sampler2D normalTexture;
uniform sampler2D glossinessTexture;
uniform sampler2D specularTexture;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec4 lightColor;

uniform float ambientStrength;
uniform float diffuseStrength;
uniform float specularStrength;

void main()
{
    vec3 T = normalize(v_tangent);
    vec3 B = normalize(v_bitangent);
    vec3 N = normalize(v_normal);
    mat3 TBN = mat3(T, B, N);

    vec3 rawNormal = texture(normalTexture, v_texcoord).xyz * 2.0 - 1.0;
    vec3 n = normalize(TBN * rawNormal);

    vec3 l = normalize(lightPos - v_worldPos);
    vec3 v = normalize(viewPos - v_worldPos);
    vec3 h = normalize(v + l);

    float diff = max(dot(n, l), 0.0);

    float glossValue = texture(glossinessTexture, v_texcoord).r;
    float shininess = exp2(glossValue * 8.0);

    float specFactor = pow(max(dot(n, h), 0.0), shininess);

    if (diff <= 0.0) {
        specFactor = 0.0;
    }

    vec4 specColor = vec4(texture(specularTexture, v_texcoord).rgb, 1.0);
    vec4 specular = lightColor * specColor * specFactor * specularStrength;

    vec4 baseColor = texture(colorTexture, v_texcoord);
    vec4 ambient = baseColor * ambientStrength;
    vec4 diffuse = baseColor * lightColor * diff * diffuseStrength;

    color = ambient + diffuse + specular;
}