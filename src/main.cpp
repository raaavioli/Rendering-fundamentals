#include <glm/glm.hpp>

#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <helpers.h>
#include <window.h>
#include <scene.h>

/* TODO: Include custom scenes here */
#include <example_scene.h>

int main(void)
{
    Scene* scenes[] = {
        new ExampleScene(),
        /* TODO: Add new scenes here */
    };

    const int scene_count = sizeof(scenes) / sizeof(scenes[0]);
    int current_scene = 0;

    glfwSetErrorCallback(ErrorCallback);
    if (!glfwInit())
        exit(EXIT_FAILURE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL 

    const uint32_t WIDTH = 640;
    const uint32_t HEIGHT = 480;
    GLFWwindow* glfwWindow = glfwCreateWindow(WIDTH, HEIGHT, "DH2323 Rendering", NULL, NULL);
    if (!glfwWindow)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwSetKeyCallback(glfwWindow, KeyCallback);
    glfwMakeContextCurrent(glfwWindow);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "ERROR: failed to initialize OpenGL context \n" << std::endl;
        exit(EXIT_FAILURE);
    }

    GL_CHECK(glClearColor(1.0, 0.0, 0.0, 1.0));

    int width, height;
    glfwGetFramebufferSize(glfwWindow, &width, &height);

    Window appWindow(WIDTH, HEIGHT);
    appWindow.Clear(glm::vec3(0.0, 0.0, 1.0));

    uint32_t backbufferTexture;
    GL_CHECK(glGenTextures(1, &backbufferTexture));
    GL_CHECK(glBindTexture(GL_TEXTURE_2D, backbufferTexture));
    GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
    GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
    GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
    GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
    GL_CHECK(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_FLOAT, appWindow.GetBufferPtr()));
    GL_CHECK(glBindTexture(GL_TEXTURE_2D, 0));

    uint32_t backbufferShader = CreateShader(s_VertexShaderText, s_FragmentShaderText);
    uint32_t textureLocation = glGetUniformLocation(backbufferShader, "u_Texture");

    // Create empty VAO since OpenGL requires a vertex array to be bound to call glDrawArrays
    // However this VAO has no attached buffers, since it is only used to draw 6 vertices 
    // without attributes (fullscreen quad)
    uint32_t emptyVAO = 0;
    if (!emptyVAO)
        GL_CHECK(glGenVertexArrays(1, &emptyVAO));
    GL_CHECK(glBindVertexArray(emptyVAO));
   
    float time = (float) glfwGetTime();
    while (!glfwWindowShouldClose(glfwWindow))
    {
        glfwGetFramebufferSize(glfwWindow, &width, &height);
        GL_CHECK(glViewport(0, 0, width, height));
        GL_CHECK(glClear(GL_COLOR_BUFFER_BIT));

        float current_time = (float) glfwGetTime();
        float dt = current_time - time;
        time = current_time;

        // Update and draw to backbuffer in lab implementation
        scenes[current_scene]->Update(dt);
        scenes[current_scene]->Draw(appWindow);

        // Present backbuffer updated in lab code
        glUseProgram(backbufferShader);
        const uint32_t activeTexture = 0;
        GL_CHECK(glActiveTexture(GL_TEXTURE0 + activeTexture));
        GL_CHECK(glBindTexture(GL_TEXTURE_2D, backbufferTexture));
        GL_CHECK(glTexSubImage2D(
            GL_TEXTURE_2D,              // target
            0,                          // level
            0,                          // xoffset
            0,                          // yoffset
            appWindow.GetWidth(), 
            appWindow.GetHeight(),      // width, height
            GL_RGB,                     // image format
            GL_FLOAT,                   // pixel data type
            appWindow.GetBufferPtr()    // data
        ));

        GL_CHECK(glUniform1i(textureLocation, activeTexture));
        GL_CHECK(glDrawArrays(GL_TRIANGLES, 0, 6));

        glfwSwapBuffers(glfwWindow);
        glfwPollEvents();
    }

    glfwDestroyWindow(glfwWindow);

    glfwTerminate();
    exit(EXIT_SUCCESS);
}