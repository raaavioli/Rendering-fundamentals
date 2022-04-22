#pragma once

#include <stdlib.h>
#include <iostream>

#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

static const char* s_GLSLVersion = "#version 410";

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