#version 410 core

layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 aUV;

out vec2 vUV;

uniform mat3 uModel;
uniform mat3 uProjection;

// Sprite-sheet UV transform
uniform vec2 uUVScale;
uniform vec2 uUVOffset;

void main()
{
    vec3 world = uModel * vec3(aPos, 1.0);
    vec3 proj  = uProjection * world;
    gl_Position = vec4(proj.xy, 0.0, 1.0);

    // apply atlas transform to base UVs
    vUV = aUV * uUVScale + uUVOffset;
}