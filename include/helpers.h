#pragma once

#include <stdlib.h>
#include <iostream>

#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "window.h"

static const char* s_GLSLVersion = "#version 410";


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

void ReallocateTexture(uint32_t* texture, const Window& window)
{
    if (*texture)
        GL_CHECK(glDeleteTextures(1, texture));
    *texture = 0;
    GL_CHECK(glGenTextures(1, texture));
    GL_CHECK(glBindTexture(GL_TEXTURE_2D, *texture));
    GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
    GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
    GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
    GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
    GL_CHECK(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, window.GetWidth(), window.GetHeight(), 0, GL_RGB, GL_FLOAT, window.GetBufferPtr()));
    GL_CHECK(glBindTexture(GL_TEXTURE_2D, 0));
}