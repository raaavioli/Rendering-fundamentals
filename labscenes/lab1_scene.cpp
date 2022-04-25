#include "lab1_scene.h"

#include <random>

void Lab1Scene::InitStarfield(uint32_t n)
{
	m_Stars.resize(n);

	// TODO: To be implemented in Lab 1.2.1, see spec in lab1_scene.h
}

void Lab1Scene::Update(float dt)
{
	if (m_DrawColors)
	{
		// No animation or updating is required here.
	} 
	else 
	{
		// TODO: To be implemented in Lab 1.2 Starfall
	}
}

void Lab1Scene::Draw(Window& window)
{
	if (m_DrawColors)
	{
		// TODO: To be changed in task 1.1.3
		// - Input colors manipulated in DrawGUI, opposed to static colors.
		DrawColors(window, 
			glm::vec3(1.0, 0.0, 0.0), 
			glm::vec3(0.0, 0.0, 1.0), 
			glm::vec3(0.0, 1.0, 1.0), 
			glm::vec3(0.0, 1.0, 0.0) 
		);
	}
	else
	{
		window.Clear(glm::vec3(0.0f));
		DrawStarfield(window, (float) window.GetWidth(), (float) window.GetHeight(), window.GetHeight() / 2.0f, glm::vec3(1.0f));
	}
}

void Lab1Scene::DrawGUI()
{
	ImGui::Checkbox("Draw colors", &m_DrawColors);
	// TODO: To be implemented in Lab 1.1.3, see spec in lab1_scene.h

	// TODO: To be implemented in Lab 1.2.4, see spec in lab1_scene.h
}

template<typename T>
void Lab1Scene::Interpolate(T a, T b, std::vector<T>& result)
{
	// TODO: To be implemented in Lab 1.1.1, see spec in lab1_scene.h
}

void Lab1Scene::DrawColors(Window& window, glm::vec3 top_left, glm::vec3 top_right, glm::vec3 bottom_left, glm::vec3 bottom_right)
{
	// TODO: To be implemented in Lab 1.1.2, see spec in lab1_scene.h

}

void Lab1Scene::DrawStarfield(Window& window, float W, float H, float f, glm::vec3 star_color)
{
	// TODO: To be implemented in Lab 1.2.3, see spec in lab1_scene.h
}

