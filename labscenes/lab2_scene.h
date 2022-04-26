#pragma once
#include <vector>

#include "scene.h"


class Lab2Scene : public Scene {
public:
	Lab2Scene() {  };

	void Draw(Window& window) override;

	/**
	*
	*/
	void Update(float dt) override;

	/**
	*
	*/
	void DrawGUI() override;

	inline const char* GetName() override { return "Lab-2-Raytracing"; };

private:

};