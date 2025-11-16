#version 330 core

in vec2 TexCoord;  // From vertex shader
out vec4 FragColor;

uniform sampler2D spriteTexture;
uniform vec4 spriteColor;  // Tint color (default: white)
uniform float alpha;       // Transparency

void main() {
    vec4 texColor = texture(spriteTexture, TexCoord);
    FragColor = texColor * spriteColor * vec4(1.0, 1.0, 1.0, alpha);
}
