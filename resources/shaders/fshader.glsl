#version 330 core

in vec2 v_texcoord;

out vec4 color;

uniform sampler2D colorTexture;
uniform sampler2D normalTexture;

void main()
{
    color = texture2D(colorTexture, v_texcoord);
}