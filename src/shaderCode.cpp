const char *vertexShaderSource = R"glsl(
    #version 330 core
     
     layout(location = 0) in vec2 position;
     layout(location = 1) in vec2 texUV;
     
     uniform vec2 modelPos;

     out vec2 texCoords;
     out vec2 objectPosition;

     void main(){
        vec3 newPosition = vec3(position.xy + modelPos, 0);
        gl_Position = vec4(newPosition, 1.0);
        texCoords = texUV;
        objectPosition = position;
    }
    )glsl";

const char *fragmentShaderSource = R"glsl(
    #version 330 core

    in vec2 texCoords;
    in vec2 objectPosition;

    uniform sampler2D textureSampler;
    uniform bool useTexture; 

    out vec4 fragmentColor;
    
    void main() {
            // vec4 background = texture(textureSampler, texCoords);

            if (useTexture) {
                fragmentColor = texture(textureSampler, texCoords); // Or any other color
            } else {
                fragmentColor = vec4(1.0,1.0,1.0,1.0);
                // fragmentColor = vec4(1.0,0.5,0.5,1.0);
                // fragmentColor = mix(vec4(1.0, 0.5, 0.2, 0.5), background, 1); // Or any other color
                // fragmentColor = vec4(vec3(1.0) - texture(textureSampler, objectPosition).rgb,1.0); // Or any other color
            //     fragmentColor = texture(textureSampler, TexCoords);
            }
        }
        )glsl";

const char *textFragmentShaderSource = R"glsl(
    #version 330 core

    in vec2 texCoords;
    in vec2 objectPosition;

    uniform sampler2D textureSampler;

    out vec4 fragmentColor;
    
    void main() {
            fragmentColor = texture(textureSampler, texCoords); // Or any other color
            fragmentColor = vec4(1.0,1.0,1.0,fragmentColor.r);
        }
        )glsl";
