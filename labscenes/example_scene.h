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

	void Update(float dt) override;
	void Draw(Window& window) override;

private:
	glm::vec3 GetSphereColor(glm::vec2 UV);

private:
	Sphere m_Sphere;
	glm::vec3 m_LightPos;
};