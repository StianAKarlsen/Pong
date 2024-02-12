#version 330 core

in vec2 objectPosition;
in vec2 texCoords;
in vec2 texCoordsss;

uniform sampler2D screenTexture;

out vec4 fragmentColor;

void main() {
    // vec4 bgtexture = texture(screenTexture, vec2(texCoordsss.x,texCoordsss.y) );
    vec4 bgtexture = texture(screenTexture, texCoordsss);
    vec4 objectColor = vec4(0.83, 0.83, 0.83, 1.0);
    vec4 objectColor1 = vec4(0.0, 1.0, 0.0, 1.0);
    vec4 objectColor2 = vec4(0.39, 0.0, 0.91, 1.0);

    // fragmentColor = bgtexture;
    fragmentColor = mix(bgtexture,objectColor2,0.5);
    // fragmentColor = objectColor1;
    if(bgtexture.a > 0)
    fragmentColor = objectColor2;
    // fragmentColor = (bgtexture*objectColor2);
    // fragmentColor = mix(objectColor2,bgtexture,0.4);
    // fragmentColor = fragmentColor + bgtexture;
    // if(bgtexture.r != 0 && bgtexture.g != 0 && bgtexture.b != 0 && bgtexture.a != 0)
        // fragmentColor = objectColor2;
         
        // fragmentColor = objectColor1;
        // fragmentColor = mix(bgtexture,fragmentColor);
        // fragmentColor = bgtexture;
    // fragmentColor = fragmentColor +bgtexture + fragmentColor*bgtexture ;

}