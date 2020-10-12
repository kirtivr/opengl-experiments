#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shader.h"

#include <iostream>
#include <cmath>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

using experimentgl::Shader;

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

    std::unique_ptr<Shader> shader = Shader::Create("vertex_shaders/triangle.vs", "fragment_shaders/triangle.fs");
    shader->use();

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
      // input
      // -----
      processInput(window);

      // Do processing.
      // Give the uniform var "ourValue" its value.
      float tv1 = glfwGetTime();
      float r1 = (sin(tv1) / 2.0f) + 0.5f;
      float g1 = (cos(tv1) / 2.0f) + 0.5f;
      float b1 = (sin(tv1 * 2) / 2.0f) + 0.5f;
      float tv2 = glfwGetTime();
      float r2 = (cos(tv2) / 2.0f) + 0.5f;
      float g2 = (sin(tv2) / 2.0f) + 0.5f;
      float b2 = (cos(tv2 * 2) / 2.0f) + 0.5f;

      float vertices[] = {
        // positions          // colors
        -0.75f, 0.75f, 0.0f, r1, g1, b1,
        0.0f, -0.75f, 0.0f,   0.0f, 0.0f, 0.0f,
        0.75f, 0.75f, 0.0f,  r2, g2, b2,
      };
      // Bind VAO.
      unsigned int VAO[1];
      glGenVertexArrays(1, VAO);
      // Buffer type of a vertex buffer is GL_ARRAY_BUFFER.
      // Bind newly created buffer to the GL_ARRAY_BUFFER target.
      unsigned int VBO[1];
      glGenBuffers(1, VBO);

      float texCoords[] = {
        0.0f, 0.0f, // lower left corner for textures.
        1.0f, 0.0f, // lower right.
        0.5f, 1.0f  // upper center.
      };
      // Texture wrapping. What happens if we specify texture co-ordinates outside of 0.0f to 1.0f?.
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
      // Triangle setup.
      glBindVertexArray(VAO[0]);
      glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
      // Copy vertices[] to buffer's memory using glBufferData.
      // glBufferDtata is a function used to copy user-defined data into the "currently bound" buffer.
      glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
      glVertexAttribPointer(/* attribute location = 0 */ 0, /* size of attribute */ 3,
                            GL_FLOAT, /*data to be normalized?*/ GL_FALSE,
                            /* stride or space b/w consecutive vertex attributes*/ 6 * sizeof(float),
                            /* offset where position begins in the buffer */ (void*)0);
      glEnableVertexAttribArray(0);
      glVertexAttribPointer(/* attribute location = 1 */ 1, /* size of attribute */ 3,
                            GL_FLOAT, /*data to be normalized?*/ GL_FALSE,
                            /* stride or space b/w consecutive vertex attributes*/ 6 * sizeof(float),
                            /* offset where position begins in the buffer */ (void*)(3* sizeof(float)));
      glEnableVertexAttribArray(1);
      // Rendering commands here.
      glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT);
      shader->setFloat("rightShiftOffset", 0.0f);
      shader->use();
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
