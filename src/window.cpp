#include "window.h"

#include <string.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

Window::Window(uint32_t width, uint32_t height) : m_Width(width), m_Height(height)
{
	m_BackBuffer = new glm::vec3[width * height];
}

Window::~Window()
{
	delete[] m_BackBuffer;
}

void Window::PutPixel(uint32_t x, uint32_t y, glm::vec3 color)
{
	if (x < m_Width && y < m_Height)
	{
		m_BackBuffer[y * m_Width + x] = color;
	}
	else
	{
		// Check this to help students if they accidentally write OOB.
		std::cerr << "ERROR: Cannot write pixel (" << x << ", " << y << ")" << " on a window of resolution " << m_Width << "x" << m_Height << std::endl;
		exit(EXIT_FAILURE);
	}
}

void Window::Clear(glm::vec3 color)
{
	for (uint32_t y = 0; y < m_Height; y++)
	{
		for (uint32_t x = 0; x < m_Width; x++)
		{
			m_BackBuffer[y * m_Width + x] = color;
		}
	}
}

void Window::Resize(uint32_t width, uint32_t height)
{
	if (m_BackBuffer)
		delete[] m_BackBuffer;
	m_BackBuffer = new glm::vec3[width * height];
	m_Width = width;
	m_Height = height;
}

const float* Window::GetBufferPtr() const
{
	return (float*)m_BackBuffer;
}

void Window::TakeScreenshot(const char* filename)
{
	char* bitmap = new char[m_Width * m_Height * 3];
	for (uint32_t i = 0; i < m_Width * m_Height; i++)
	{
		glm::vec3 color = glm::clamp(m_BackBuffer[i], glm::vec3(0), glm::vec3(1));
		bitmap[i * 3 + 0] = (char) (color.x * 255);
		bitmap[i * 3 + 1] = (char) (color.y * 255);
		bitmap[i * 3 + 2] = (char) (color.z * 255);
	}
	std::string filename_png = std::string(filename) + ".png";
	stbi_write_png(filename_png.c_str(), m_Width, m_Height, 3, bitmap, m_Width * 3);
	delete[] bitmap;
}