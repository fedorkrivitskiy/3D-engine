#version 330 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec2 a_texcoord;
layout(location = 2) in vec3 a_normal;
layout(location = 3) in vec3 a_tangent;

out vec2 v_texcoord;
out vec3 v_normal;
out vec3 v_tangent;
out vec3 v_bitangent;
out vec3 v_worldPos;

uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;

void main()
{
    vec4 worldPos = model_matrix * vec4(a_position, 1.0);
    gl_Position = projection_matrix * view_matrix * worldPos;

    v_texcoord = a_texcoord;
    v_worldPos = worldPos.xyz;

    mat3 normalMatrix = transpose(inverse(mat3(model_matrix)));

    vec3 T = normalize(normalMatrix * a_tangent);
    vec3 N = normalize(normalMatrix * a_normal);

    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T);

    v_tangent = T;
    v_bitangent = B;
    v_normal = N;
}
