// #AI generert

#ifndef _SHADERMANAGER_HPP_
#define _SHADERMANAGER_HPP_

#include "defines.hpp"

#include <string>
#include <map>
#include <vector>
#include <functional>
#include <filesystem>

class ShaderManager {
public:
    static ShaderManager& getInstance() {
        static ShaderManager instance;
        return instance;
    }

    std::shared_ptr<ShaderProgram> getShaderProgram(const std::map<GLenum, std::string>& shaders);

private:
    ShaderManager() {} // Constructor is private for Singleton pattern
    ShaderManager(const ShaderManager&) = delete;
    ShaderManager& operator=(const ShaderManager&) = delete;

    std::map<std::string, std::shared_ptr<ShaderProgram>> shaderCache;
    std::string generateKey(const std::map<GLenum, std::string>& shaders);
};


class ShaderProgram {
public:
    ShaderProgram(const std::map<GLenum, std::string>& shaders);
    ~ShaderProgram();

    void use();
    void reload();
    void checkAndReloadUpdatedShaders();
    void setErrorCallback(std::function<void(const std::string&)> callback);

private:
    GLuint programID;
    std::map<GLenum, std::pair<std::string, std::filesystem::file_time_type>> shaderFiles;
    std::vector<struct ShaderFileInfo> updateQueue;
    std::function<void(const std::string&)> errorCallback;

    GLuint loadShaderFromFile(const std::string& path, GLenum type);
    GLuint loadShaderFromString(const std::string& source, GLenum type);
    GLuint compileShader(const std::string& source, GLenum type);
    void checkShaderError(GLuint shader, GLenum status, bool isProgram);
    void reloadShader(GLenum shaderType, const std::string& shaderPath);
    void updateLastModifiedTimes();
};

#endif // _SHADERMANAGER_HPP_

