#pragma once

#include "window.h"
#include "imgui.h"

class Scene
{
public:
	virtual void Update(float dt) = 0;
	virtual void Draw(Window& window) = 0;
	virtual void DrawGUI() = 0;

	virtual const char* GetName() = 0;
};