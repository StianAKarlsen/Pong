// #AI generert + meg

#ifndef _SHADERMANAGER_HPP_
#define _SHADERMANAGER_HPP_

#include "defines.hpp"

// Helper structure for shader file information
struct ShaderFileInfo
{
    GLenum type;
    std::string path;
    std::filesystem::file_time_type lastModifiedTime;

    ShaderFileInfo(GLenum shaderType, const std::string &shaderPath)
        : type(shaderType), path(shaderPath), lastModifiedTime(std::filesystem::last_write_time(shaderPath)) {}

    bool operator<(const ShaderFileInfo &other) const
    {
        return this->lastModifiedTime < other.lastModifiedTime;
    }
};

class ShaderProgram
{
public:
    ShaderProgram(const ShaderProgram &) = delete;            // No copy constructor
    ShaderProgram &operator=(const ShaderProgram &) = delete; // No copy assignment

    ShaderProgram &operator=(ShaderProgram &&other) noexcept
    {
        if (this != &other)
        {
            glDeleteProgram(programID);  // Free existing resource
            programID = other.programID; // Transfer ownership
            shaderFiles = other.shaderFiles;
            uniformLocations = other.uniformLocations;
            other.uniformLocations.clear();
            other.shaderFiles.clear();
            other.programID = 0; // Invalidate the old object
        }
        return *this;
    };

    ShaderProgram(){};
    ShaderProgram(const std::map<GLenum, std::string> &shaders);
    ~ShaderProgram();

    //    void setUniform(const std::string& name, void* value);
    //      Look in ShaderProgramManager_SetUniforms.hpp

    GLuint use();
    void reload();
    void checkAndReloadUpdatedShaders();
    // void setErrorCallback(std::function<void(const std::string &)> callback);

private:
    void cacheStructuredUniformLocationsAndTypes();
    GLuint programID;

    // name of uniform, pair<uniform location, uniform type>
    std::unordered_map<std::string, std::pair<GLint, GLenum>> uniformLocations;

    std::map<GLenum, std::pair<std::string, std::filesystem::file_time_type>> shaderFiles;
    std::vector<ShaderFileInfo> updateQueue;

    GLuint loadShaderFromFile(const std::string &path, GLenum type);
    GLuint loadShaderFromString(const std::string &source, GLenum type);
    GLuint compileShader(const std::string &source, GLenum type);
    void checkShaderError(GLuint shader, GLenum status, bool isProgram);
    void reloadShader(GLenum shaderType, const std::string &shaderPath);
    void updateLastModifiedTimes();

public:
    void setUniform(const std::string &name, const void *value)
    {
        // Existing uniform setting logic
        GLint &location = uniformLocations[name].first;
        GLenum &type = uniformLocations[name].second;
        // Example case for a texture (sampler2D)

        switch (type)
        {
        case GL_FLOAT:
            glUniform1f(location, *(static_cast<const GLfloat *>(value)));
            break;
        case GL_SAMPLER_2D:
        case GL_INT:
            glUniform1i(location, *(static_cast<const GLint *>(value)));
            break;
        case GL_FLOAT_VEC2:
            glUniform2fv(location, 1, static_cast<const GLfloat *>(value));
            break;
        case GL_FLOAT_VEC3:
            glUniform3fv(location, 1, static_cast<const GLfloat *>(value));
            break;
        case GL_FLOAT_VEC4:
            glUniform4fv(location, 1, static_cast<const GLfloat *>(value));
            break;
        case GL_FLOAT_MAT4:
            glUniformMatrix4fv(location, 1, GL_FALSE, static_cast<const GLfloat *>(value));
            break;
        // Add cases for other types as needed
        default:
            std::cerr << "Unsupported uniform type for " << name << std::endl;
            break;
        }
    }

    void setTexture(const std::string &name, GLuint textureID, GLint textureUnit)
    {
        glActiveTexture(GL_TEXTURE0 + textureUnit);
        glBindTexture(GL_TEXTURE_2D, textureID); // Adjust for other texture types
        setUniform(name, &textureUnit);
    }
};

//----------------------------------------------------------------------------------------------------------------------

class ShaderManager
{
public:
    // Disallow copying and assignment
    ShaderManager(const ShaderManager &) = delete;
    ShaderManager &operator=(const ShaderManager &) = delete;

    // Access the singleton instance of the ShaderManager
    static ShaderManager &getInstance();

    // Methods to manage shader programs
    ShaderProgram &createShaderProgram(const std::string &name, const std::map<GLenum, std::string> &shaderPaths);
    ShaderProgram *getShaderProgram(const std::string &name);
    void deleteShaderProgram(const std::string &name);

    void reloadShaderIfFileChange();

private:
    // Private constructor for singleton pattern
    ShaderManager(){};

    // Container for managing shader program instances
    std::unordered_map<std::string, ShaderProgram> shaderPrograms;
};

#endif // _SHADERMANAGER_HPP_
