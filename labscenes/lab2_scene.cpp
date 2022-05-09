#include "lab2_scene.h"

#include "input.h"

Lab2Scene::Lab2Scene() : m_RenderNext(true), m_SingleFrameMode(false)
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

	for (int y = 0; y < window.GetHeight(); ++y)
	{
		for (int x = 0; x < window.GetWidth(); ++x)
		{
			// TODO: To be implemented in Lab 2.1.1 (direction)
			// TODO: To be implemented in Lab 2.1.4 (rotation)
			// Tips: Trace a Ray from the camera's position through each pixel on the screen.
			//		-	A vector d going from the camera's start position through pixel (x, y) 
			//			can then be computed with: 
			//				d = (x - W / 2, y - H / 2, f)
			//			Where W is the width of the screen, H is the height of the screen, f is the focal length.
			//			As in Lab1, you can use f = H / 2 to get 90 degrees vertical FOV.
			//		-	d is in the camera's local space. Use the camera's rotation matrix to transform from camera
			//			local space to world space.
			Ray ray = {};

			// Find the closest intersection of the casted ray.
			Intersection intersection = {};
			if (ClosestIntersection(ray, m_Model, intersection)) 
			{
				window.PutPixel(x, y, DirectLight(m_Light, intersection, m_Model));
			}
			else 
			{
				window.PutPixel(x, y, glm::vec3(0.5, 0.73, 0.93));
			}
		}
	}
}

void Lab2Scene::Update(float dt)
{
	// TODO: To be implemented in Lab 2.1.4, see spec in lab1_scene.h
	// Tips: See input.h
	// Example:
	//	if (Input::IsKeyPressed(Key::A)) 
	//	{
	//		// Move camera left
	//	}
}

void Lab2Scene::DrawGUI()
{
	ImGui::Checkbox("Single frame mode", &m_SingleFrameMode);
	if (!m_SingleFrameMode || m_SingleFrameMode && ImGui::Button("Render")) 
		m_RenderNext = true;

	// Add any GUI-features you may need for your implementation below.
}

bool Lab2Scene::ClosestIntersection(const Ray& ray, const std::vector<Triangle>& triangles, Intersection& intersection)
{
	// TODO: To be implemented in Lab 2.1.2, see spec in lab1_scene.h
	// Tips: 
	//		- Initialize the intersection distance to a large value (std::numeric_limits<float>::max())
	//		- A matrix glm::mat3 can be initialize with vectors x, y and z like: glm::mat3 A(x, y, z)
	//		- To get the inverse of glm::mat3 A, use glm::inverse(A)
	//		- Ensure that intersection always contains the closest distance, position and index of the closest intersecting triangle.

	return false;
}

glm::vec3 Lab2Scene::DirectLight(const Light& light, const Intersection& i, const std::vector<Triangle>& triangles) 
{
	// TODO: To be implemented in Lab 2.1.3, see spec in lab1_scene.h
	// Tips:
	//		- Always use normalized direction vectors for lighting calculations
	//		- Casting a shadow ray can be done using ClosestIntersection again, but with the intersection point as ray start
	//			in the direction to the light source.
	//		- The inverse square law affects both light coming from the light source hitting the surface as well as 
	//			light coming from the surface hitting the camera.

	// Cast shadow ray

	// Calculate diffuse, specular and ambient colors

	// Combine into a single output light color

	return glm::vec3(1.0f);
}

glm::mat3 Camera::GetRotationY() const
{
	// TODO: To be implemented in Lab 2.1.4, see spec in lab1_scene.h
	return glm::mat3(
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f
	);
}
