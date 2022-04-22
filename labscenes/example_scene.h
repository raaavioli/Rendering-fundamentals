#pragma once

#include "scene.h"

struct Sphere {
	float radius;
	glm::vec2 position;
};

class ExampleScene : public Scene {
public:
	ExampleScene() 
	{
		m_Sphere.position = glm::vec2(0.0, 0.0);
		m_Sphere.radius = 0.25f;
		m_LightPos = glm::vec3(0.0, 0.0, 4.0f);
	}

	/**
	* Update data in scene
	* - Runs before Draw(Window& window)
	* @param dt Delta time in seconds
	*/
	void Update(float dt) override;

	/**
	* Draw scene to Window
	* - Draw scene pixel by pixel to window, typically by invoking Window::PutPixel(x, y, color)
	* - Runs after Update(float dt)
	* - Runs before DrawGUI()
	* @param window Window in which to draw
	*/
	void Draw(Window& window) override;

	/**
	* Draw ImGui user interface (https://github.com/ocornut/imgui)
	* - Allows drawing sliders, buttons etc in a separate panel specific for this scene
	* - Runs inbetween ImGui::Begin and ImGui::After, meaning no setup code is needed.
	* - Example methods: ImGui::Text("Scene text"); ImGui::SliderFloat("Number", ...);
	* - Runs after Draw(Window& window)
	*/
	void DrawGUI() override;

	inline const char* GetName() override { return "ExampleScene"; };

private:
	glm::vec3 GetSphereColor(glm::vec2 UV);

private:
	Sphere m_Sphere;
	glm::vec3 m_LightPos;
};