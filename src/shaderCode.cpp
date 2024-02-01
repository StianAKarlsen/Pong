
// vec4 background = texture(textureSampler, texCoords);vertexShaderSource
const char *vertexShaderSource = R"glsl(
    #version 330 core

layout(location = 0) in vec2 position;
layout(location = 1) in vec2 texUV;

uniform vec2 modelPos;

out vec2 texCoords;
out vec2 objectPosition;

void main() {
vec3 newPosition = vec3(position.xy + modelPos, 0);
gl_Position = vec4(newPosition, 1.0);
texCoords = texUV;
objectPosition = position;
}
)glsl";

// vec4 background = texture(textureSampler, texCoords);
const char *fragmentShaderSource = R"glsl(
    #version 330 core

in vec2 texCoords;
in vec2 objectPosition;

uniform sampler2D textureSampler;

out vec4 fragmentColor;

void main() {

fragmentColor = vec4(1.0, 1.0, 1.0, 1.0);
                // fragmentColor = vec4(1.0,0.5,0.5,1.0);
                // fragmentColor = mix(vec4(1.0, 0.5, 0.2, 0.5), background, 1); // Or any other color
                // fragmentColor = vec4(vec3(1.0) - texture(textureSampler, objectPosition).rgb,1.0); // Or any other color
            //     fragmentColor = texture(textureSampler, TexCoords);
}
)glsl";

//     fragmentColor = texture(textureSampler, TexCoords);
const char *textFragmentShaderSource = R"glsl("
    #version 330 core

in vec2 texCoords;
in vec2 objectPosition;

uniform sampler2D textureSampler;

out vec4 fragmentColor;

void main() {
fragmentColor = texture(textureSampler, texCoords); // Or any other color
fragmentColor = vec4(1.0, 0.0, 1.0, fragmentColor.r);
}
)glsl";

/// background/framebuffer/fullscreen image
const char *imageFragmentShaderSource = R"glsl(
    #version 330 core
out vec4 FragColor;
uniform sampler2D screenTexture;

void main() {
vec2 normalizedCoords = gl_FragCoord.xy / vec2(gl_FramebufferWidth, gl_FramebufferHeight);
FragColor = texture(screenTexture, normalizedCoords);
}
)glsl";