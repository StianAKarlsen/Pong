#version 330 core

in vec2 objectPosition;
in vec2 texCoords;

uniform sampler2D screenTexture;

out vec4 fragmentColor;

void main() {
    // fragmentColor = texture(screenTexture, gl_FragCoord.xy);
    // vec2 normalizedCoords = gl_FragCoord.xy / vec2(gl_FramebufferWidth, gl_FramebufferHeight);
    fragmentColor = texture(screenTexture, texCoords);
    // fragmentColor.r = 0;
    // fragmentColor.g = 0;
    // fragmentColor.b = 0;
    // fragmentColor = fragmentColor * vec4(0.0, 0.18, 1.0, 1.0);
}