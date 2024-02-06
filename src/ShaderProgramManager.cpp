#include "ShaderProgramManager.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <chrono>
#include <thread>

// Helper structure for shader file information
struct ShaderFileInfo {
    GLenum type;
    std::string path;
    std::filesystem::file_time_type lastModifiedTime;

    ShaderFileInfo(GLenum shaderType, const std::string& shaderPath)
        : type(shaderType), path(shaderPath), lastModifiedTime(std::filesystem::last_write_time(shaderPath)) {}

    bool operator<(const ShaderFileInfo& other) const {
        return this->lastModifiedTime < other.lastModifiedTime;
    }
};

ShaderProgram::ShaderProgram(const std::map<GLenum, std::string>& shaders) {
    programID = glCreateProgram();
    for (const auto& shader : shaders) {
        auto compiledShader = loadShaderFromFile(shader.second, shader.first);
        glAttachShader(programID, compiledShader);
        glDeleteShader(compiledShader); // Delete shader after attaching
    }
    glLinkProgram(programID);
    checkShaderError(programID, GL_LINK_STATUS, true);
}

ShaderProgram::~ShaderProgram() {
    glDeleteProgram(programID);
}

void ShaderProgram::use() {
    glUseProgram(programID);
}

void ShaderProgram::reload() {
    for (auto& [type, pair] : shaderFiles) {
        auto& [path, _] = pair; // Ignore the last modified time here
        reloadShader(type, path);
    }
}

void ShaderProgram::checkAndReloadUpdatedShaders() {
    updateLastModifiedTimes();
    for (auto& fileInfo : updateQueue) {
        reloadShader(fileInfo.type, fileInfo.path);
    }
    updateQueue.clear(); // Clear the queue after reloading shaders
}

void ShaderProgram::setErrorCallback(std::function<void(const std::string&)> callback) {
    errorCallback = callback;
}

GLuint ShaderProgram::loadShaderFromFile(const std::string& path, GLenum type) {
    std::ifstream shaderFile(path);
    std::stringstream shaderData;
    shaderData << shaderFile.rdbuf();
    shaderFile.close();
    return compileShader(shaderData.str(), type);
}

GLuint ShaderProgram::compileShader(const std::string& source, GLenum type) {
    GLuint shader = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);
    checkShaderError(shader, GL_COMPILE_STATUS, false);
    return shader;
}

void ShaderProgram::checkShaderError(GLuint shader, GLenum status, bool isProgram) {
    GLint success;
    GLchar infoLog[1024];
    if (isProgram) {
        glGetProgramiv(shader, status, &success);
        if (!success) {
            glGetProgramInfoLog(shader, sizeof(infoLog), nullptr, infoLog);
            if (errorCallback) errorCallback(std::string(infoLog));
        }
    } else {
        glGetShaderiv(shader, status, &success);
        if (!success) {
            glGetShaderInfoLog(shader, sizeof(infoLog), nullptr, infoLog);
            if (errorCallback) errorCallback(std::string(infoLog));
        }
    }
}

void ShaderProgram::reloadShader(GLenum shaderType, const std::string& shaderPath) {
    GLuint newShader = loadShaderFromFile(shaderPath, shaderType);
    // Replace the old shader with the new shader in the program
    glAttachShader(programID, newShader);
    glLinkProgram(programID); // Re-link program with the new shader
    glDeleteShader(newShader); // Delete the new shader as it's now attached to the program

    checkShaderError(programID, GL_LINK_STATUS, true);
}

void ShaderProgram::updateLastModifiedTimes() {
    // Iterate over shaderFiles to check for updates
    for (auto& [type, pair] : shaderFiles) {
        auto& [path, lastModifiedTime] = pair;
        auto currentModifiedTime = std::filesystem::last_write_time(path);
        if (currentModifiedTime > lastModifiedTime) {
            updateQueue.emplace_back(type, path);
            lastModifiedTime = currentModifiedTime;
        }
    }

    // Sort updateQueue to have the most recently modified files at the end
    std::sort(updateQueue.begin(), updateQueue.end());

    // Keep only the last 3 updated files
    if (updateQueue.size() > 3) {
        updateQueue.erase(updateQueue.begin(), updateQueue.end() - 3);
    }
}



std::shared_ptr<ShaderProgram> ShaderManager::getShaderProgram(const std::map<GLenum, std::string>& shaders) {
    std::string key = generateKey(shaders);
    auto it = shaderCache.find(key);
    if (it != shaderCache.end()) {
        // Found existing shader program, return it
        return it->second;
    } else {
        // Create a new shader program, cache it, and return it
        auto shaderProgram = std::make_shared<ShaderProgram>(shaders);
        shaderCache[key] = shaderProgram;
        return shaderProgram;
    }
}

std::string ShaderManager::generateKey(const std::map<GLenum, std::string>& shaders) {
    std::stringstream keyStream;
    for (const auto& shader : shaders) {
        keyStream << shader.first << ":" << shader.second << ";";
    }
    return keyStream.str();
}
