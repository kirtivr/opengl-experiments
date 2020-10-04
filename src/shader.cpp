#include "shader.h"
#include <fstream>
#include <memory>
#include <sstream>
#include <string>
#include <iostream>

namespace experimentgl {

namespace {

std::string load_file(const std::string& path) {
  std::ifstream shader(path);
  std::stringstream ss;
  ss << shader.rdbuf();
  shader.close();
  return ss.str();
}

} // anonymous namespace.

Shader::Shader(std::string v_path, std::string fr_path): v_path_(v_path), fr_path_(fr_path){}

std::unique_ptr<Shader> Shader::Create(std::string v_path, std::string fr_path) {
  std::unique_ptr<Shader> s (new Shader(v_path, fr_path));
  if (!s->Init()) {
    std::cout << "Could not initialize shaders!";
    return nullptr;
  }
  return s;
}

unsigned int Shader::CompileShader(unsigned int shader_type, const char* source) {
  unsigned int shader;
  int success;
  char info_log[512];

  switch (shader_type) {
    case GL_VERTEX_SHADER:
      // vertex shader.
      shader = glCreateShader(GL_VERTEX_SHADER);
      break;
    case GL_FRAGMENT_SHADER:
      // fragment shader.
      shader = glCreateShader(GL_FRAGMENT_SHADER);
      break;
    default:
      std::cout << "Invalid shader type given";
      // TODO(kirtivr): use StatusOr once I figure out CMake.
      return 0;
  }
  glShaderSource(shader, 1, &source, NULL);
  glCompileShader(shader);
  // print compile errors, if any.
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(shader, 512, NULL, info_log);
    std::cout << "shader compile failed: " << info_log << std::endl;
  }
  return shader;
}

bool Shader::CompileProgram(unsigned int vertex, unsigned int fragment) {
  // Shader program.
  id_ = glCreateProgram();
  glAttachShader(id_, vertex);
  glAttachShader(id_, fragment);
  glLinkProgram(id_);
  // print linking errors if any.
  int success;
  glGetProgramiv(id_, GL_LINK_STATUS, &success);
  if (!success) {
    char info_log[512];
    glGetProgramInfoLog(id_, 512, NULL, info_log);
    std::cout << "program compile failed: " << info_log << std::endl;
    return false;
  }
  // delete the shaders.
  glDeleteShader(vertex);
  glDeleteShader(fragment);
  return true;
}

bool Shader::Init() {
  std::string v_code = load_file(v_path_);
  std::string fr_code = load_file(fr_path_);
  // Compile shaders.
  unsigned int vertex = CompileShader(GL_VERTEX_SHADER, v_code.c_str());
  unsigned int fragment = CompileShader(GL_FRAGMENT_SHADER, fr_code.c_str());
  std::cout<< "vertex shader: " << v_code << std::endl;
  std::cout<< "fragment shader: " << v_code << std::endl;
  if (!CompileProgram(std::move(vertex), std::move(fragment))) {
    return false;
  }
  return true;
}

void Shader::use() {
  glUseProgram(id_);
}

void Shader::setBool(const std::string &name, bool value) const
{
  glUniform1i(glGetUniformLocation(id_, name.c_str()), (int)value);
}

void Shader::setInt(const std::string &name, int value) const
{
  glUniform1i(glGetUniformLocation(id_, name.c_str()), value);
}

void Shader::setFloat(const std::string &name, float value) const
{
  glUniform1f(glGetUniformLocation(id_, name.c_str()), value);
}

}
