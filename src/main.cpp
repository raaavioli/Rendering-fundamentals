#include <glm/glm.hpp>

#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <stdlib.h>
#include <iostream>

#include <window.h>
#include <lab1.h>
#include <lab2.h>
#include <lab3.h>

static const char* s_VertexShaderText = R"(
#version 330

vec2 vertices[] = {
  vec2(-1.0, -1.0),
  vec2(1.0, -1.0),
  vec2(1.0, 1.0),
  vec2(1.0, 1.0),
  vec2(-1.0, 1.0),
  vec2(-1.0, -1.0)
};

layout(location = 0) out vec2 out_UV;

void main() {
  out_UV = (vertices[gl_VertexID] + 1) / 2; 
  gl_Position = vec4(vertices[gl_VertexID], 0.0, 1.0);
}
)";

static const char* s_FragmentShaderText = R"(
#version 330

layout(location = 0) in vec2 in_UV;
layout(binding = 0) uniform sampler2D u_Texture;

out vec4 out_Color;

void main() {
    out_Color = texture(u_Texture, in_UV);
}
)";


static void ErrorCallback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void CheckOpenGLError(const char* stmt, const char* fname, int line)
{
    GLenum err = glGetError();
    if (err != GL_NO_ERROR)
    {
        printf("OpenGL error %08x, at %s:%i - for %s\n", err, fname, line, stmt);
        exit(EXIT_FAILURE);
    }
}

#define GL_CHECK(stmt) do { \
            stmt; \
            CheckOpenGLError(#stmt, __FILE__, __LINE__); \
        } while (0)

int main(void)
{
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

    GL_CHECK(glClearColor(0.0, 0.0, 1.0, 1.0));

    int width, height;
    glfwGetFramebufferSize(glfwWindow, &width, &height);

    Window appWindow(WIDTH, HEIGHT);
    appWindow.Clear(glm::vec3(1.0, 0.0, 1.0));

    uint32_t glTexture;
    GL_CHECK(glGenTextures(1, &glTexture));
    GL_CHECK(glBindTexture(GL_TEXTURE_2D, glTexture));
    GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
    GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
    GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
    GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
    GL_CHECK(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, appWindow.GetBufferPtr()));

    uint32_t glVertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(glVertexShader, 1, &s_VertexShaderText, NULL);
    glCompileShader(glVertexShader);

    uint32_t glFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(glFragmentShader, 1, &s_FragmentShaderText, NULL);
    glCompileShader(glFragmentShader);

    uint32_t glShaderProgram = glCreateProgram();
    glAttachShader(glShaderProgram, glVertexShader);
    glAttachShader(glShaderProgram, glFragmentShader);
    glLinkProgram(glShaderProgram);

    while (!glfwWindowShouldClose(glfwWindow))
    {
        GL_CHECK(glViewport(0, 0, width, height));
        GL_CHECK(glClear(GL_COLOR_BUFFER_BIT));

        Lab1::Update();
        Lab1::Draw(appWindow);

        GL_CHECK(glBindTexture(GL_TEXTURE_2D, glTexture));
        GL_CHECK(glTexSubImage2D(
            GL_TEXTURE_2D,              // target
            0,                          // level
            0,                          // xoffset
            0,                          // yoffset
            width, height,              // width, height
            GL_RGB,                     // image format
            GL_FLOAT,                   // pixel data type
            appWindow.GetBufferPtr()    // data
        ));


        glfwSwapBuffers(glfwWindow);
        glfwPollEvents();
    }

    glfwDestroyWindow(glfwWindow);

    glfwTerminate();
    exit(EXIT_SUCCESS);
}