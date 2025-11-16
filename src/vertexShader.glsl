#version 330 core

layout (location = 0) in vec2 aPos;       // Vertex position (x, y)
layout (location = 1) in vec2 aTexCoord;  // Texture coordinates

out vec2 TexCoord;  // Pass to fragment shader

uniform mat4 projection;  // Orthographic projection matrix
uniform mat4 model;       // Position, rotation, scale

void main() {
    gl_Position = projection * model * vec4(aPos, 0.0, 1.0);
    TexCoord = aTexCoord;
}