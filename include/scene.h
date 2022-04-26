#pragma once

#include "window.h"
#include "imgui.h"

class Scene
{
public:
	virtual ~Scene() {};

	/**
	* Update data in scene
	* - Runs before Draw(Window& window)
	* @param dt Delta time in seconds
	*/
	virtual void Update(float dt) = 0;

	/**
	* Draw scene to Window
	* - Draw scene pixel by pixel to window, typically by invoking Window::PutPixel(x, y, color)
	* - Runs after Update(float dt)
	* - Runs before DrawGUI()
	* @param window Window in which to draw
	*/
	virtual void Draw(Window& window) = 0;

	/**
	* Draw ImGui user interface (https://github.com/ocornut/imgui)
	* - Allows drawing sliders, buttons etc in a separate panel specific for this scene
	* - Runs inbetween ImGui::Begin and ImGui::After, meaning no setup code is needed. 
	* - Example methods: ImGui::Text("Scene text"); ImGui::SliderFloat("Number", ...); 
	* - Runs after Draw(Window& window)
	*/
	virtual void DrawGUI() = 0;

	virtual const char* GetName() = 0;
};