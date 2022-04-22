#include <glm/glm.hpp>

#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <stdlib.h>
#include <iostream>

#include <window.h>
#include <scene.h>
#include <example_scene.h>

static const char* s_VertexShaderText = R"(
#version 410

vec2 vertices[] = vec2[6](
  vec2(-1.0, -1.0),
  vec2(1.0, -1.0),
  vec2(1.0, 1.0),
  vec2(1.0, 1.0),
  vec2(-1.0, 1.0),
  vec2(-1.0, -1.0));

layout(location = 0) out vec2 out_UV;

void main() {
  // Flip y to get (0,0) in top left corner
  out_UV = vec2(1.0, -1.0) * (vertices[gl_VertexID] + 1.0f) / 2.0f; 
  gl_Position = vec4(vertices[gl_VertexID], 0.0, 1.0);
}
)";

static const char* s_FragmentShaderText = R"(
#version 410

layout(location = 0) in vec2 in_UV;

out vec4 out_Color;

uniform sampler2D u_Texture;

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

void CheckShaderCompilationError(uint32_t shader, const char* type)
{
    int success = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {

        char infoLog[512];
        glGetShaderInfoLog(shader, sizeof(infoLog), nullptr, &infoLog[0]);
        glDeleteShader(shader);

        printf("ERROR: Failed to compile \"%s\"\n", type);
        printf("Log: %s\n", infoLog);
        exit(EXIT_FAILURE);
    }
}

void CheckProgramLinkError(uint32_t program, const char* type)
{
    int success = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success)
    {

        char infoLog[512];
        glGetProgramInfoLog(program, sizeof(infoLog), nullptr, &infoLog[0]);
        glDeleteProgram(program);

        printf("ERROR: Failed to link program \"%s\"\n", type);
        printf("Log: %s\n", infoLog);
        exit(EXIT_FAILURE);
    }
}

#define GL_CHECK(stmt) do { \
            stmt; \
            CheckOpenGLError(#stmt, __FILE__, __LINE__); \
        } while (0)

/**
* Creates an OpenGL shader program from a glsl vertex and fragment shader source code
* @param vsSrc GLSL vertex shader source code
* @param fsSrc GLSL fragment shader source code
*/
uint32_t CreateShader(const char* vsSrc, const char* fsSrc)
{
    uint32_t vertexShader = glCreateShader(GL_VERTEX_SHADER);
    GL_CHECK(glShaderSource(vertexShader, 1, &vsSrc, NULL));
    GL_CHECK(glCompileShader(vertexShader));
    CheckShaderCompilationError(vertexShader, "Backbuffer vertex shader");

    uint32_t fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    GL_CHECK(glShaderSource(fragmentShader, 1, &fsSrc, NULL));
    GL_CHECK(glCompileShader(fragmentShader));
    CheckShaderCompilationError(fragmentShader, "Backbuffer fragment shader");

    uint32_t shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    CheckProgramLinkError(shaderProgram, "Backbuffer shader");
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

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

    Scene* scenes[] = {
        new ExampleScene(),
        /* TODO: Add new scenes here */
    };
   
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
        scenes[0]->Update(dt);
        scenes[0]->Draw(appWindow);

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