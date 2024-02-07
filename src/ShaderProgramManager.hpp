// #AI generert

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
    ShaderProgram(const ShaderProgram&) = delete; // No copy constructor
    ShaderProgram& operator=(const ShaderProgram&) = delete; // No copy assignment

    ShaderProgram& operator=(ShaderProgram&& other) noexcept {
        if (this != &other) {
            glDeleteProgram(programID); // Free existing resource
            programID = other.programID; // Transfer ownership
            other.programID = 0; // Invalidate the old object
        }
        return *this;
    };

    ShaderProgram() {};
    ShaderProgram(const std::map<GLenum, std::string> &shaders);
    ~ShaderProgram();

    GLuint use();
    void reload();
    void checkAndReloadUpdatedShaders();
    void setErrorCallback(std::function<void(const std::string &)> callback);

private:
    GLuint programID;
    std::map<GLenum, std::pair<std::string, std::filesystem::file_time_type>> shaderFiles;
    std::vector<ShaderFileInfo> updateQueue;
    std::function<void(const std::string &)> errorCallback;

    GLuint loadShaderFromFile(const std::string &path, GLenum type);
    GLuint loadShaderFromString(const std::string &source, GLenum type);
    GLuint compileShader(const std::string &source, GLenum type);
    void checkShaderError(GLuint shader, GLenum status, bool isProgram);
    void reloadShader(GLenum shaderType, const std::string &shaderPath);
    void updateLastModifiedTimes();
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

private:
    // Private constructor for singleton pattern
    ShaderManager() {};

    // Container for managing shader program instances
    std::unordered_map<std::string, ShaderProgram> shaderPrograms;
};

#endif // _SHADERMANAGER_HPP_
