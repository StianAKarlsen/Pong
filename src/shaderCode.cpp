
// vec4 background = texture(textureSampler, texCoords);vertexShaderSource
const char *defaultVertexShaderSource = R"glsl(
#version 330 core

layout(location = 0) in vec2 position;
layout(location = 1) in vec2 texUV;

uniform vec2 modelPos;

out vec2 objectPosition;
out vec2 texCoords;

void main() {
vec3 newPosition = vec3(position.xy + modelPos, 0);
gl_Position = vec4(newPosition, 1.0);
texCoords = texUV;
objectPosition = position;
}
)glsl";

const char *defaultFragmentShaderSource = R"glsl(
#version 330 core

in vec2 objectPosition;
in vec2 texCoords;

out vec4 fragmentColor;

void main() {
fragmentColor = vec4(1.0, 1.0, 1.0, 1.0);
}
)glsl";

const char *textFragmentShaderSource = R"glsl(
#version 330 core

in vec2 objectPosition;
in vec2 texCoords;

uniform sampler2D textureSampler;

out vec4 fragmentColor;

void main() {
vec4 afragmentColor = texture(textureSampler, texCoords); // Or any other color
fragmentColor = vec4(1.0, 1.0, 1.0, afragmentColor.r);
}
)glsl";

// /// background/framebuffer/fullscreen image
// const char *imageFragmentShaderSource = R"glsl(
//     #version 330 core
// uniform sampler2D screenTexture;
// out vec4 FragColor;

// void main() {
// vec2 normalizedCoords = gl_FragCoord.xy / vec2(gl_FramebufferWidth, gl_FramebufferHeight);
// FragColor = texture(screenTexture, normalizedCoords);
// }
// )glsl";