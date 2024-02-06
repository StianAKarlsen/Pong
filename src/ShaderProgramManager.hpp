// #AI generert

#ifndef _SHADERMANAGER_HPP_
#define _SHADERMANAGER_HPP_

#include <string>
#include <unordered_map>
#include <vector>
#include <GL/glew.h>

// Represents a single shader file.
struct ShaderFile {
    std::string path;
    GLenum type;
    time_t lastModified = 0;
};

// Represents a shader program consisting of multiple shaders.
struct ShaderProgram {
    GLuint programID;
    std::vector<ShaderFile> shaders;
    std::unordered_map<GLenum, GLuint> shaderIDs; // Maps shader types to shader object IDs
};

class ShaderProgramManager {
public:
    ShaderProgramManager();
    ~ShaderProgramManager();

    ShaderProgramManager(const ShaderProgramManager&) = delete;
    ShaderProgramManager& operator=(const ShaderProgramManager&) = delete;

    // Registers a shader program with the given shaders. og returner programid eller 0 neo noe feil compilert(se error log/console), eller ingen endring i filene/programmet.
    GLuint registerShaderProgram(const std::vector<ShaderFile>& shaders);

    // Use a specific shader program.
    void useProgram(GLuint programID);

    // Checks for updates in any shader files and reloads them if necessary.
    void reloadShadersIfNeeded();

private:
    std::unordered_map<GLuint, ShaderProgram> shaderPrograms;

    std::chrono::time_point<std::chrono::steady_clock> currentTime, lastTime;
    std::chrono::seconds refreshRate;
    // Compiles a shader from source, returns shader ID.
    GLuint compileShader(const std::string& shaderPath, GLenum shaderType);

    // Links shaders into a program, returns program ID.
    GLuint linkShadersIntoProgram(const std::vector<GLuint>& shaderIDs);

    // Checks if the shader file has been modified.
    bool isShaderFileModified(const ShaderFile& shaderFile);

    // Updates the timestamp of a shader file.
    void updateShaderFileTimestamp(ShaderFile& shaderFile);

    // Cleans up resources for a shader program.
    void cleanupShaderProgram(GLuint programID);

    // Detach and delete shaders.
    void detachAndDeleteShaders(GLuint programID, const std::vector<GLuint>& shaderIDs);
};

#endif // _SHADERMANAGER_HPP_
