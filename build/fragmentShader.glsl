#version 410 core

in vec2 vUV;
out vec4 FragColor;

uniform sampler2D spriteTexture;
uniform vec4 uTint;

void main()
{
    vec4 tex = texture(spriteTexture, vUV);
    FragColor = tex * uTint;
}