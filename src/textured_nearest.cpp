#include <glad/glad.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
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

    std::unique_ptr<Shader> shader = Shader::Create("vertex_shaders/triangle_texture.vs", "fragment_shaders/triangle_texture.fs");
    shader->use();

    // Create texture.
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // Texture wrapping. What happens if we specify texture co-ordinates outside of 0.0f to 1.0f?.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    // Texel to pick from given float co-ordinates. Go for GL_NEAREST for a more "8-bit" look.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    int width, height, nr_channels;
    unsigned char *data = stbi_load("textures/texture_d.png", &width, &height, &nr_channels, 0);
    if (data) {
      // Generate the texture with loaded image.
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
      // Generate all required mipmaps for currently bound texture.
      glGenerateMipmap(GL_TEXTURE_2D);
    } else {
      std::cout << "Failed to load texture!." << std::endl;
    }
    // Can free image memory now!.
    stbi_image_free(data);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
      // input
      // -----
      processInput(window);

      // Bind VAO.
      unsigned int VAO;
      glGenVertexArrays(1, &VAO);
      // Buffer type of a vertex buffer is GL_ARRAY_BUFFER.
      // Bind newly created buffer to the GL_ARRAY_BUFFER target.
      unsigned int VBO;
      glGenBuffers(1, &VBO);
      glBindVertexArray(VAO);

      // Add texture co-ordinates to vertices.
      float vertices[] = {
        // positions          // colors           // texture coords
        0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
        0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left
      };
      // Copy vertices[] to buffer's memory using glBufferData.
      // glBufferDtata is a function used to copy user-defined data into the "currently bound" buffer.
      glBindBuffer(GL_ARRAY_BUFFER, VBO);
      glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
      glVertexAttribPointer(/* attribute location = 0 */ 0, /* size of attribute */ 3,
                            GL_FLOAT, /*data to be normalized?*/ GL_FALSE,
                            /* stride or space b/w consecutive vertex attributes*/ 8 * sizeof(float),
                            /* offset where position begins in the buffer */ (void*)0);
      glEnableVertexAttribArray(0);
      glVertexAttribPointer(/* attribute location = 1 */ 1, /* size of attribute */ 3,
                            GL_FLOAT, /*data to be normalized?*/ GL_FALSE,
                            /* stride or space b/w consecutive vertex attributes*/ 8 * sizeof(float),
                            /* offset where position begins in the buffer */ (void*)(3* sizeof(float)));
      glEnableVertexAttribArray(1);
      glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
      glEnableVertexAttribArray(2);
      // Rendering commands here.
      glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT);
      glBindTexture(GL_TEXTURE_2D, texture);
      shader->use();
      glBindVertexArray(VAO);
      glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
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
