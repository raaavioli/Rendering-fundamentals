#include "window.h"

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

const float* Window::GetBufferPtr()
{
	return (float*)m_BackBuffer;
}

void Window::TakeScreenshot(const char* filename)
{
	// TODO: Implement
}