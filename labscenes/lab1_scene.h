#pragma once

#include "scene.h"

class Lab1Scene : public Scene {
public:
	Lab1Scene(){}

	void Update(float dt) override;
	void Draw(Window& window) override;
	void DrawGUI() override;

	inline const char* GetName() override { return "Lab-1"; };

private:

};