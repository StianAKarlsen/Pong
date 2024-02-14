#version 330 core

layout(location = 0) in vec2 position;
layout(location = 1) in vec2 texUV;

uniform vec2 modelPos;

out vec2 objectPosition;
out vec2 texCoords;
out vec2 texCoordsss;

void main() {
    vec3 newPosition = vec3(position.xy + modelPos, 0);
    gl_Position = vec4(newPosition, 1.0);
    texCoords = texUV;

    vec2 bboxMin = vec2(-1.0, 1.0);
    vec2 bboxMax = vec2(1.0, -1.0);

    objectPosition = newPosition.xy;
    texCoordsss = (objectPosition - bboxMin) / (bboxMax - bboxMin);
}