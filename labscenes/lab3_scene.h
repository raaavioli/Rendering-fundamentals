#pragma once
#include <vector>

#include "scene.h"


class Lab3Scene : public Scene {
public:
	Lab3Scene() {  };

	void Draw(Window& window) override;


	void Update(float dt) override;

	void DrawGUI() override;

	inline const char* GetName() override { return "Lab-3-Rasterization"; };

private:

};