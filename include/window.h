#pragma once
#include <iostream>

#include <glm/glm.hpp>

class Window
{
public:
	Window(uint32_t width, uint32_t height);
	~Window();

	void PutPixel(uint32_t x, uint32_t y, glm::vec3 color);
	void Clear(glm::vec3 color);
	void Resize(uint32_t width, uint32_t height);

	inline uint32_t GetWidth() { return m_Width; }
	inline uint32_t GetHeight() { return m_Height; }

	void TakeScreenshot(const char* filename);

	// Used to draw the Backbuffer with OpenGL
	const float* GetBufferPtr();

private:
	uint32_t m_Width;
	uint32_t m_Height;
	glm::vec3* m_BackBuffer;
};