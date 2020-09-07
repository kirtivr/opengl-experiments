#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

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
                                     "  gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
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
                                   "out vec4 FragColor;\n\n"
                                   "void main()\n"
                                   "{\n"
                                   "  FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
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
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
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
      0.5f,  0.5f, 0.0f,  // top right
      0.5f, -0.5f, 0.0f,  // bottom right
      -0.5f, -0.5f, 0.0f,  // bottom left
      -0.5f,  0.5f, 0.0f   // top left
    };
    unsigned int indices[] = {
      0, 1, 3,   // first triangle
      1, 2, 3    // second triangle
    };

    // Bind VAO. Order matters!.
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    // Buffer type of a vertex buffer is GL_ARRAY_BUFFER.
    // Bind newly created buffer to the GL_ARRAY_BUFFER target.
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // Copy vertices[] to buffer's memory using glBufferData.
    // glBufferDtata is a function used to copy user-defined data into the "currently bound" buffer.
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // Bind EBO.
    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Set the vertex attribute pointers.
    glVertexAttribPointer(/* attribute location = 0 */ 0, /* size of attribute */ 3,
                          GL_FLOAT, /*data to be normalized?*/ GL_FALSE,
                          /* stride or space b/w consecutive vertex attributes*/ 3 * sizeof(float),
                          /* offset where position begins in the buffer */ (void*)0);
    // Vertex attributes are disabled by default.
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
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE /* GL_FILL */);
        glUseProgram(sp);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        // glBindVertexArray(0);
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
