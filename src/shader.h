#ifndef SHADER_H_
#define SHADER_H_

#include <glad/glad.h>  // include glad to get all the required OpenGL headers

#include <memory>
#include <string>

namespace experimentgl {

class Shader {
public:
  // program ID
  unsigned int id_;

  // Creates, initializes and returns the Shader.
  static std::unique_ptr<Shader> Create(std::string vertex_path, std::string fragment_path);
  // activate the shader.
  void use();
  // util uniform functions.
  void setBool(const std::string& name, bool value) const;
  void setInt(const std::string& name, int value) const;
  void setFloat(const std::string& name, float value) const;

 private:
  // Private ctor to force construction through Create().
  Shader(std::string vertex_path, std::string fragment_path);
  // returns 'false' if shader was not initialized.
  bool Init();
  // Given shader type and source, compile shader and return its id.
  unsigned int CompileShader(unsigned int shader_type, const char* source);
  // Compile program and store id_.
  bool CompileProgram(unsigned int vertex, unsigned int fragment);
  // Path to the vertex shader file.
  std::string v_path_;
  // Path to the vertex shader file.
  std::string fr_path_;
};

}
#endif // SHADER_H_
