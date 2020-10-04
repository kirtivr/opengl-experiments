#version 330 core
layout (location=0) in vec3 aPos;
layout (location=1) in vec3 aColor;

out vec3 ourColor;
uniform float rightShiftOffset;

void main() {
  vec4 position = vec4(rightShiftOffset + aPos.x, aPos.y, aPos.z, 1.0f);
  gl_Position = position;
  // ourColor = aColor;
  ourColor = vec3(position.x, position.y, position.z);
}
