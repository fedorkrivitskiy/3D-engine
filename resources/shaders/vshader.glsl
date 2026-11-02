#version 330 core

layout(location = 0) in vec4 a_position;
layout(location = 1) in vec2 a_texcoord;
layout(location = 2) in vec3 a_normal;
layout(location = 3) in vec3 a_tangent;

out vec2 v_texcoord;
out vec3 v_normal;
out vec3 v_light;

uniform mat4 mvp_matrix;

void main()
{
    gl_Position = mvp_matrix * a_position;

    v_texcoord = a_texcoord;
    v_normal = normalize(a_normal);
    v_light = normalize(vec3(1.0, 0.0, 0.0));
}
