#pragma once

#include "window.h"

class Scene
{
public:
	virtual void Update(float dt) = 0;
	virtual void Draw(Window& window) = 0;
};