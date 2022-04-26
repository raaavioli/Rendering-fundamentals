#include "lab2_scene.h"

Lab2Scene::Lab2Scene() : m_RenderNext(true), m_SingleFrameMode(true)
{
	LoadTestModel(m_Model);
	m_Light.position = glm::vec3(0.0f, 0.0f, 0.0);
	m_Light.color = glm::vec3(1.f, 1.f, 1.f);
	m_Camera.position = glm::vec3(0.0, 0.0, -2.0);
	m_Camera.yaw = 0.0f;
}

void Lab2Scene::Draw(Window& window)
{
	if (!m_RenderNext) return;
	m_RenderNext = false;

	for (uint32_t y = 0; y < window.GetHeight(); ++y)
	{
		for (uint32_t x = 0; x < window.GetWidth(); ++x)
		{
			// Trace a Ray from the camera's position through the pixel on the screen.
			//		-	A vector d pointing in the direction where the light 
			//			reaching pixel (x, y) comes from can then be computed as :
			//				d = (x - W / 2, y - H / 2, f)
			//			Where W is the width of the screen, H is the height of the screen, f is the focal length.
			//			As in Lab1, you can use f = H / 2 to get 90 degrees vertical FOV.

			// TODO: To be implemented in Lab 2.x.x
			Ray ray = {};

			// Find the closest intersection of the casted ray.
			Intersection intersection = {};
			if (ClosestIntersection(ray, m_Model, intersection)) 
			{
				window.PutPixel(x, y, DirectLight(m_Light, intersection, m_Model));
			}
			else 
			{
				window.PutPixel(x, y, glm::vec3(0.0f));
			}
		}
	}
}

void Lab2Scene::Update(float dt)
{
	// TODO: To be implemented in Lab 2.x.x, see spec in lab1_scene.h
}

void Lab2Scene::DrawGUI()
{
	ImGui::Checkbox("Single frame mode", &m_SingleFrameMode);
	if (!m_SingleFrameMode || m_SingleFrameMode && ImGui::Button("Render")) 
		m_RenderNext = true;

	// TODO: To be implemented in Lab 2.x.x, see spec in lab1_scene.h
}

bool Lab2Scene::ClosestIntersection(const Ray& ray, const std::vector<Triangle>& triangles, Intersection& intersection)
{
	// TODO: To be implemented in Lab 2.x.x, see spec in lab1_scene.h

	return false;
}

glm::vec3 Lab2Scene::DirectLight(const Light& light, const Intersection& i, const std::vector<Triangle>& triangles) 
{
	// TODO: To be implemented in Lab 2.x.x, see spec in lab1_scene.h

	return glm::vec3(1.0f);
}

glm::mat3 Camera::GetRotation()
{
	// TODO: To be implemented in Lab 2.x.x, see spec in lab1_scene.h
	return glm::mat3(
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f
	);
}
