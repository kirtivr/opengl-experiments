#ifndef SHADER_H_
#define SHADER_H_

#include <glad/glad.h>  // include glad to get all the required OpenGL headers

#include <string>

namespace experimentgl {

class Shader {
public:
  // program ID
  unsigned int ID;

  // Creates and initializes the Shader.
  bool Create(std::string vertex_path, std::string fragment_path);
  // activate the shader.
  void use();
  // util uniform functions.
  void setBool(const std::string& name, bool value) const;
  void setInt(const std::string& name, int value) const;
  void setFloat(const std::string& name, float value) const;

 private:
  // Path to the vertex shader file.
  std::string v_path_;
  // Path to the vertex shader file.
  std::string fr_path_;
};

}
#endif // SHADER_H_
