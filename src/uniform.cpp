#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <cmath>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

unsigned int compileVertexShader() {
  // Source for vertex shader.
  const char *vertex_shader_source = "#version 330 core\n"
                                     "layout (location = 0) in vec3 aPos;\n"
                                     "void main()\n"
                                     "{\n"
                                     "  gl_Position = vec4(aPos, 1.0);\n"
                                     "}\0";
  unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  // Attach the shader source code to the shader object and compile the shader.
  glShaderSource(vertex_shader, /*how many strings are we passing*/ 1, &vertex_shader_source, NULL);
  glCompileShader(vertex_shader);

  // Check if compilation was successful.
  int success;
  char  msg[512];
  glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vertex_shader, 512, NULL, msg);
    std::cout << "VERTEX SHADER COMPILATION FAILED\n" << msg << std::endl;
  }
  return vertex_shader;
}

unsigned int compileFragmentShader() {
  const char *frag_shader_source = "#version 330 core\n"
                                   // Output variable FragColor.
                                   "out vec4 FragColor;\n"
                                   "uniform vec4 ourColor;\n"
                                   "void main()\n"
                                   "{\n"
                                   "  FragColor = ourColor;\n"
                                   "}\n\0";
  unsigned int frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
  // Attach the shader source code to the shader object and compile the shader.
  glShaderSource(frag_shader, /*how many strings are we passing*/ 1, &frag_shader_source, NULL);
  glCompileShader(frag_shader);

  // Check if compilation was successful.
  int success;
  char  msg[512];
  glGetShaderiv(frag_shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(frag_shader, 512, NULL, msg);
    std::cout << "FRAG SHADER COMPILATION FAILED\n" << msg << std::endl;
  }
  return frag_shader;
}

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Experiments", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    float vertices[] = {
      -0.25f, -0.25f, 0.0f,
      0.0f, 0.0f, 0.0f,
      0.25f, -0.25f, 0.0f,
    };
    // Bind VAO.
    unsigned int VAO[1];
    glGenVertexArrays(1, VAO);
    // Buffer type of a vertex buffer is GL_ARRAY_BUFFER.
    // Bind newly created buffer to the GL_ARRAY_BUFFER target.
    unsigned int VBO[1];
    glGenBuffers(1, VBO);

    // First triangle setup.
    glBindVertexArray(VAO[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    // Copy vertices[] to buffer's memory using glBufferData.
    // glBufferDtata is a function used to copy user-defined data into the "currently bound" buffer.
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(/* attribute location = 0 */ 0, /* size of attribute */ 3,
                          GL_FLOAT, /*data to be normalized?*/ GL_FALSE,
                          /* stride or space b/w consecutive vertex attributes*/ 3 * sizeof(float),
                          /* offset where position begins in the buffer */ (void*)0);
    glEnableVertexAttribArray(0);
    // Give vertex attribute location as argument.
    glEnableVertexAttribArray(0);
    unsigned int vs = compileVertexShader();
    unsigned int fs = compileFragmentShader();
    // Shader program: multiple shaders combined.
    // Links output of each shader to the input of the next shader.
    unsigned int sp = glCreateProgram();
    glAttachShader(sp, vs);
    glAttachShader(sp, fs);
    glLinkProgram(sp);
    // Check if linking was successful.
    int success;
    char msg[512];
    glGetShaderiv(sp, GL_LINK_STATUS, &success);
    if (!success) {
      glGetShaderInfoLog(sp, 512, NULL, msg);
      std::cout << "SHADER LINK FAILED\n" << msg << std::endl;
    }
    glUseProgram(sp);

    // Once linked we do not need them anymore.
    glDeleteShader(vs);
    glDeleteShader(fs);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        // Rendering commands here.
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        // Give the uniform var "ourValue" its value.
        float timeValue = glfwGetTime();
        float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
        int vertexColorLocation = glGetUniformLocation(sp, "ourColor");
        if (vertexColorLocation == -1) {
          std::cout << "Could not get vertex color location" << std::endl;
        }
        glUseProgram(sp);
        // updating a uniform does require you to first use the program (by calling glUseProgram),
        // because it sets the uniform on the currently active shader program.
        glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
        glBindVertexArray(VAO[0]);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        // glDrawArrays(GL_TRIANGLES, 0, 3);
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}
