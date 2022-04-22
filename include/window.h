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

	void TakeScreenshot(const char* filename);

	const float* GetBufferPtr();

private:
	uint32_t m_Width;
	uint32_t m_Height;
	glm::vec3* m_BackBuffer;
};