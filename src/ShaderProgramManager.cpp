// #AI generert.

#include "defines.hpp"

#include "ShaderProgramManager.hpp"

#include <fstream>
#include <thread>
#include <sstream>
#include <iostream>
#include <sys/stat.h> // For stat(), to get file modification times

ShaderProgramManager::ShaderProgramManager()
{
  lastTime = std::chrono::steady_clock::now();

  std::chrono::seconds refreshRate = std::chrono::seconds(60);
}

ShaderProgramManager::~ShaderProgramManager()
{
  // Delete all shader programs on destruction
  for (auto &entry : shaderPrograms)
  {
    glDeleteProgram(entry.first);
  }
}

GLuint ShaderProgramManager::registerShaderProgram(const std::vector<ShaderFile> &shaders)
{
  std::vector<GLuint> shaderIDs;
  bool needToLink = false;

  // Attempt to compile each shader
  for (const auto &shaderFile : shaders)
  {
    if (isShaderFileModified(shaderFile))
    {
      GLuint shaderID = compileShader(shaderFile.path, shaderFile.type);
      if (shaderID != 0)
      { // Compilation success
        shaderIDs.push_back(shaderID);
        needToLink = true;
      }
      else
      {
        std::cerr << "Failed to compile shader: " << shaderFile.path << std::endl;
      }
    }
  }

  // Only proceed to link if there are new or updated shaders
  if (needToLink)
  {
    GLuint programID = linkShadersIntoProgram(shaderIDs);
    if (programID != 0)
    {
      // If linking succeeded, update program map and clean up
      ShaderProgram program = {programID, shaders};
      shaderPrograms[programID] = program;
      for (auto &shader : program.shaders)
      {
        updateShaderFileTimestamp(shader);
      }
      detachAndDeleteShaders(programID, shaderIDs);
      return programID;
    }
    else
    {
      std::cerr << "Failed to link shader program." << std::endl;
    }
  }
  return 0;
}

void ShaderProgramManager::useProgram(GLuint programID)
{
  // if (shaderPrograms.find(programID) != shaderPrograms.end())
  // {
  glUseProgram(programID);
  // }
}

void ShaderProgramManager::reloadShadersIfNeeded()
{
  // std::thread s;
  // if(s.joinable())
  //   s.join();
  // currentTime = std::chrono::steady_clock::now();
  // auto sek = std::chrono::duration_cast<std::chrono::seconds>(currentTime - lastTime);
  // if (sek >= refreshRate)
    for (auto &entry : shaderPrograms)
    {
      // lastTime = currentTime;
      registerShaderProgram(entry.second.shaders);
    }
}

GLuint ShaderProgramManager::compileShader(const std::string &shaderPath, GLenum shaderType)
{
  std::ifstream shaderFile(shaderPath);
  if (!shaderFile.is_open())
  {
    std::cerr << "Failed to open shader file: " << shaderPath << std::endl;
    return 0;
  }
  std::stringstream shaderData;
  shaderData << shaderFile.rdbuf();
  shaderFile.close();

  std::string shaderCode = shaderData.str();
  const char *shaderSource = shaderCode.c_str();

  GLuint shaderID = glCreateShader(shaderType);
  glShaderSource(shaderID, 1, &shaderSource, nullptr);
  glCompileShader(shaderID);

  GLint success;
  glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    GLchar infoLog[512];
    glGetShaderInfoLog(shaderID, 512, nullptr, infoLog);
    std::cerr << "ERROR::SHADER::COMPILATION_FAILED\n"
              << infoLog << std::endl;

    glDeleteShader(shaderID); // Don't leak the shader.
    return 0;
  }

  return shaderID;
}

GLuint ShaderProgramManager::linkShadersIntoProgram(const std::vector<GLuint> &shaderIDs)
{
  GLuint programID = glCreateProgram();
  for (GLuint shaderID : shaderIDs)
  {
    glAttachShader(programID, shaderID);
  }
  glLinkProgram(programID);

  GLint success;
  glGetProgramiv(programID, GL_LINK_STATUS, &success);
  if (!success)
  {
    GLchar infoLog[512];
    glGetProgramInfoLog(programID, 512, nullptr, infoLog);
    std::cerr << "ERROR::PROGRAM::LINKING_FAILED\n"
              << infoLog << std::endl;
    glDeleteProgram(programID); // Don't leak the program.
    return 0;
  }

  return programID;
}

bool ShaderProgramManager::isShaderFileModified(const ShaderFile &shaderFile)
{
  struct stat fileStat;
  if (stat(shaderFile.path.c_str(), &fileStat) != 0)
  {
    std::cerr << "Failed to get file stats for: " << shaderFile.path << std::endl;
    return false;
  }
  return fileStat.st_mtime > shaderFile.lastModified;
}

void ShaderProgramManager::updateShaderFileTimestamp(ShaderFile &shaderFile)
{
  struct stat fileStat;
  if (stat(shaderFile.path.c_str(), &fileStat) == 0)
  {
    shaderFile.lastModified = fileStat.st_mtime;
  }
}

void ShaderProgramManager::cleanupShaderProgram(GLuint programID)
{
  // This function would be used to delete old programs if necessary
}

void ShaderProgramManager::detachAndDeleteShaders(GLuint programID, const std::vector<GLuint> &shaderIDs)
{
  for (GLuint shaderID : shaderIDs)
  {
    glDetachShader(programID, shaderID);
    glDeleteShader(shaderID);
  }
}
