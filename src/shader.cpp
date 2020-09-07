#include "shader.h"
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>

namespace experimentgl {

Shader(const char* v_path, const char* fr_path) {
  std::string v_code;
  std::string fr_code;
  std::ifstream v_shader;
  std::ifstream fr_shader;

  // ensure ifstream objects can throw exceptions.
  v_shader.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  fr_shader.exceptions(std::ifstream::failbit | std::ifstream::badbit);

  try {
  v_shader.open(v_path);
  fr_path.open(fr_path);
  std::stringstream v_stream, fr_stream;
  // read file's contents to stream.
  v_stream << v_shader.rdbuf();
  fr_stream << fr_shader.rdbuf();
  v_shader.close();
  fr_shader.close();

  v_code = v_stream.str();
  fr_code = fr_stream.str();
  } catch (std::ifstream::failure e) {
    std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
  }

  const char* v_shader_code = v_code.c_str();
  const char* fr_shader_code = fr_code.c_str();

  
}

}

