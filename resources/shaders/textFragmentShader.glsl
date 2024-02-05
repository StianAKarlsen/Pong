#version 330 core

in vec2 objectPosition;
in vec2 texCoords;

uniform sampler2D textureSampler;

out vec4 fragmentColor;

void main() {
    vec4 afragmentColor = texture(textureSampler, texCoords); // Or any other color
    fragmentColor = vec4(1.0, 1.0, 1.0, afragmentColor.r);
}