// #AI + meg

#include "ShaderProgramManager.hpp"

ShaderProgram::ShaderProgram(const std::map<GLenum, std::string> &shaders)
{
  programID = glCreateProgram();
  for (const auto &shader : shaders)
  {
    auto compiledShader = loadShaderFromFile(shader.second, shader.first);
    glAttachShader(programID, compiledShader);
    glDeleteShader(compiledShader); // Delete shader after attaching

    shaderFiles[shader.first] =
        std::pair<std::string, std::filesystem::file_time_type>(
            shader.second,
            std::filesystem::last_write_time(shader.second));
  }
  // updateLastModifiedTimes();
  glLinkProgram(programID);
  checkShaderError(programID, GL_LINK_STATUS, true);
  cacheStructuredUniformLocationsAndTypes();
}

ShaderProgram::~ShaderProgram()
{
  glDeleteProgram(programID);
}

void ShaderProgram::cacheStructuredUniformLocationsAndTypes()
{
  uniformLocations.clear();
  GLint numUniforms = 0;
  glGetProgramiv(programID, GL_ACTIVE_UNIFORMS, &numUniforms);

  for (GLint i = 0; i < numUniforms; ++i)
  {
    char uniformName[256]; // Buffer for uniform name
    GLsizei length;
    GLint size;  // Size of the uniform variable
    GLenum type; // Type of the uniform variable
                 // typedef void (GLAPIENTRY * PFNGLGETACTIVEATTRIBPROC) (GLuint program, GLuint index, GLsizei maxLength, GLsizei* length, GLint* size, GLenum* type, GLchar* name);
    glGetActiveUniform(programID, i, sizeof(uniformName), &length, &size, &type, uniformName);
    GLint location = glGetUniformLocation(programID, std::string(uniformName).c_str());

    // Here uniformName could be something like "Light.position"
    // This stores the location of each field of the struct as well
    uniformLocations[std::string(uniformName)] = std::pair<GLuint, GLenum>(location, type);
  }
}

// void ShaderProgram::setUniform(const std::string &name, void *value)
// {
//   GLint &location = uniformLocations[name].first;
//   GLenum &type = uniformLocations[name].second;

//   switch (type)
//   {
//   case GL_FLOAT:
//     glGetUniformivARB(location, *(GLfloat *)value);
//     break;
//   case GL_TEXTURE_2D:
//     glBindTexture(GL_TEXTURE_2D, *(GLuint *)value);
//     glUniform1i(location, *(GLint *)value);
//   case GL_INT_VEC2:
//     __glewUniform2iv(location, 1,(const GLint *)value);
//     break;
//   default:
//     break;
//   }
// }

GLuint ShaderProgram::use()
{
  glUseProgram(programID);
  return programID;
}

void ShaderProgram::reload()
{
  for (auto &[type, pair] : shaderFiles)
  {
    auto &[path, _] = pair; // Ignore the last modified time here
    reloadShader(type, path);
  }
}

void ShaderProgram::checkAndReloadUpdatedShaders()
{
  updateLastModifiedTimes();
  for (auto &fileInfo : updateQueue)
  {
    reloadShader(fileInfo.type, fileInfo.path);
  }
  updateQueue.clear(); // Clear the queue after reloading shaders
}

GLuint ShaderProgram::loadShaderFromFile(const std::string &path, GLenum type)
{
  std::ifstream shaderFile(path);
  std::stringstream shaderData;
  shaderData << shaderFile.rdbuf();
  shaderFile.close();
  return compileShader(shaderData.str(), type);
}

GLuint ShaderProgram::compileShader(const std::string &source, GLenum type)
{
  GLuint shader = glCreateShader(type);
  const char *src = source.c_str();
  glShaderSource(shader, 1, &src, nullptr);
  glCompileShader(shader);
  checkShaderError(shader, GL_COMPILE_STATUS, false);
  return shader;
}

void ShaderProgram::checkShaderError(GLuint shader, GLenum status, bool isProgram)
{
  GLint success;
  GLchar infoLog[1024];
  if (isProgram)
  {
    glGetProgramiv(shader, status, &success);
    if (!success)
    {
      glGetProgramInfoLog(shader, sizeof(infoLog), nullptr, infoLog);
      std::cerr << "ProgramInfoLog Error: " << infoLog << std::endl;
    }
  }
  else
  {
    glGetShaderiv(shader, status, &success);
    if (!success)
    {
      glGetShaderInfoLog(shader, sizeof(infoLog), nullptr, infoLog);
      std::cerr << "ShaderInfoLog Error: " << infoLog << std::endl;
    }
  }
}

void ShaderProgram::reloadShader(GLenum shaderType, const std::string &shaderPath)
{
  // Create a new shader from the file
  GLuint newShader = loadShaderFromFile(shaderPath, shaderType);

  // Detach and delete the old shader of the same type, if it exists
  GLint attachedShaders = 0;
  glGetProgramiv(programID, GL_ATTACHED_SHADERS, &attachedShaders);
  std::vector<GLuint> shaders(attachedShaders);
  glGetAttachedShaders(programID, attachedShaders, nullptr, shaders.data());

  for (GLuint oldShader : shaders)
  {
    GLint type;
    glGetShaderiv(oldShader, GL_SHADER_TYPE, &type);
    if (type == static_cast<GLint>(shaderType))
    {
      glDetachShader(programID, oldShader);
      glDeleteShader(oldShader);
      break; // Assuming only one shader of each type is attached
    }
  }

  // Attach the new shader and re-link the program
  glAttachShader(programID, newShader);
  glLinkProgram(programID);
  glDeleteShader(newShader); // Delete the new shader since it's now attached to the program

  checkShaderError(programID, GL_LINK_STATUS, true);
  cacheStructuredUniformLocationsAndTypes();
}

// void ShaderProgram::reloadShader(GLenum shaderType, const std::string& shaderPath) {
//     GLuint newShader = loadShaderFromFile(shaderPath, shaderType);
//     // Replace the old shader with the new shader in the program
//     glAttachShader(programID, newShader);
//     glLinkProgram(programID); // Re-link program with the new shader
//     glDeleteShader(newShader); // Delete the new shader as it's now attached to the program

//     checkShaderError(programID, GL_LINK_STATUS, true);
// }

void ShaderProgram::updateLastModifiedTimes()
{
  // Iterate over shaderFiles to check for updates
  for (auto &[type, pair] : shaderFiles)
  {
    auto &[path, lastModifiedTime] = pair;
    auto currentModifiedTime = std::filesystem::last_write_time(path);
    if (currentModifiedTime > lastModifiedTime)
    {
      updateQueue.emplace_back(type, path);
      lastModifiedTime = currentModifiedTime;
    }
  }

  // Sort updateQueue to have the most recently modified files at the end
  std::sort(updateQueue.begin(), updateQueue.end());

  // Keep only the last 3 updated files
  if (updateQueue.size() > 3)
  {
    updateQueue.erase(updateQueue.begin(), updateQueue.end() - 3);
  }
}

//----------------------------------------------------------------------------------------------------------------------

ShaderManager &ShaderManager::getInstance()
{
  static ShaderManager instance; // Instantiated on first use, guaranteed to be destroyed
  return instance;
}

// ShaderManager& ShaderManager::getInstance() {
//     static ShaderManager instance; // Instantiated on first use, guaranteed to be destroyed
//     return instance;
// }

ShaderProgram &ShaderManager::createShaderProgram(const std::string &name, const std::map<GLenum, std::string> &shaderPaths)
{
  // Check if a shader program with the same name already exists
  auto it = shaderPrograms.find(name);
  if (it != shaderPrograms.end())
  {
    // Optionally, handle error or update existing shader program
    throw std::runtime_error("Shader program with the given name already exists.");
  }

  // Create and insert the shader program
  ShaderProgram program(shaderPaths); // Assuming ShaderProgram can be constructed this way
  shaderPrograms[name] = std::move(program);
  return shaderPrograms[name];
}

ShaderProgram *ShaderManager::getShaderProgram(const std::string &name)
{
  auto it = shaderPrograms.find(name);
  if (it != shaderPrograms.end())
  {
    return &it->second;
  }
  return nullptr; // or throw an exception if preferred
}

void ShaderManager::deleteShaderProgram(const std::string &name)
{
  shaderPrograms.erase(name);
}

void ShaderManager::reloadShaderIfFileChange()
{
  //TODO: add cooldown..
  for (auto &[_, sp] : shaderPrograms)
    sp.reload();
}
