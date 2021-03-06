#include "Shader.h"
#include <stdexcept>

// ShaderInfo
bool ShaderData::operator< (const ShaderData& other) const 
{
  if (shaderType != other.shaderType)
  {
    return shaderType < other.shaderType;
  }
  else
  {
    return shaderPath < other.shaderPath;
  }
}

bool ShaderData::operator== (const ShaderData& other) const 
{
  return shaderPath == other.shaderPath && shaderType == other.shaderType;
}

bool ShaderData::isValidForCreation() const
{
  return !shaderPath.empty() && shaderType;
}

const std::string ShaderData::toString() const 
{
  return shaderPath + " " + std::to_string(shaderType);
}

// Shader
// non static
Shader::Shader()
  : _mShaderId(0), _mIsShaderLoaded(false), _mType(0)
{}

Shader::~Shader()
{
  deleteShader();
}

void Shader::deleteShader()
{
  if (_mIsShaderLoaded)
  {
    glDeleteShader(_mShaderId);
    Log.print<info>("Shader ", _mPath, " successfully removed!");

    _mShaderId = 0;
    _mIsShaderLoaded = false;
  }
}

void Shader::load(const std::string& shaderPath, const GLenum& shaderType)
{
  if (_mIsShaderLoaded)
  {
    Log.print<warning>("Shader is already loaded: ", shaderPath);
    return;
  }
  
  // read the file
  std::ifstream fileStream;
  fileStream.open(shaderPath);
  if (!fileStream.is_open())
  {
    Log.print<error>("Cannot open shader file: ", shaderPath);
    std::string errMsg("Cannot open shader file: " + shaderPath);
    throw std::runtime_error(errMsg);
  }
  std::stringstream buffer;
  buffer << fileStream.rdbuf();
  const std::string& shaderCode = buffer.str();
  const char* shaderCodeCstr = shaderCode.c_str();

  // initialize variables
  _mShaderId = glCreateShader(shaderType);

  // compile the shader
  glShaderSource(_mShaderId, 1, &shaderCodeCstr, NULL);
  glCompileShader(_mShaderId);

  // error checking
  int  success;
  char infoLog[512];
  glGetShaderiv(_mShaderId, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    glGetShaderInfoLog(_mShaderId, 512, NULL, infoLog);
    Log.print<Severity::error>("Compilation of shader file: ", shaderPath, " failed!");
    Log.print<Severity::error>( std::string(infoLog) );

    glDeleteShader(_mShaderId);
    _mShaderId = 0;

    std::string errMsg("Failed to compile shader: " + shaderPath);
    throw std::runtime_error(errMsg.c_str());
  }

  _mPath = shaderPath;
  _mType = shaderType;
  _mIsShaderLoaded = true;
  Log.print<Severity::info>("Shader ", shaderPath, " successfully loaded!");
}

// ShaderManager
ShaderManager::ShaderManager() {}

Shader* const ShaderManager::create(const std::string& key, const ShaderData& data)
{
  Shader* shader = new Shader();
  shader->load(data.shaderPath, data.shaderType);
  return shader;
}

void ShaderManager::destroy(Shader* const value)
{
  delete value;
}