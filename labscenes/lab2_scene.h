#pragma once
#include <vector>

#include "scene.h"
#include "test_model.h"

struct Ray {
	glm::vec3 start;
	glm::vec3 direction;
};

struct Light {
	glm::vec3 position;
	glm::vec3 color;
};

struct Intersection
{
	glm::vec3 position;
	float distance;
	int triangleIndex;
};

struct Camera {
	glm::vec3 position;
	float yaw;

	glm::mat3 GetRotation();
};

#define PI 3.1415926535f

class Lab2Scene : public Scene {
public:
	Lab2Scene();

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
	/**
	* Finds closest intersection of ray among triangles.
	* 
	* @param ray The ray to intersect with triangles
	* @param triangles The triangles to be intersected with ray
	* @param intersection The closest intersection result between ray and triangles
	* @returns true if an intersection was found, false otherwise
	*/
	bool ClosestIntersection(const Ray& ray, const std::vector<Triangle>& triangles, Intersection& intersection);

	/**
	* Calculates the direct light at intersection given a single light source and a set of triangles
	* - Casts one shadow ray from the point of intersection to the light source to determine if the 
	*	intersection is occluded with respect to the light.
	* - Approximates light intensity I with the blinn-phong shading model:
	*	I = ambient + (1 - shadow) * (diffuse * surface_color + specular * light_color)
	*	where 
	*	ambient:	The approximate global illumination
	*	shadow:		The amount of "shadowness" at the intersection
	*	diffuse:	dot(N, L), where N is the normalized normal at intersection, and L is the direction to the light source
	*	specular:	dot(R, V)^p or dot(N, H)^p, where R is the reflection vector, V is the view vector, N is the normal and H is L + V. 
					p is an exponent dictating the intensity of the specular reflection.
	* 
	* @param light A light source in the sceen to accumulate direct light from
	* @param intersection The intersection point from which to accumulate direct light
	* @param triangles The triangles in the scene to consider when calculating direct light
	* @returns the amount of direct light (r, g, b) accumulated at the point of intersection
	*/
	glm::vec3 DirectLight(const Light& light, const Intersection& intersection, const std::vector<Triangle>& triangles);

private:
	std::vector<Triangle> m_Model;
	Camera m_Camera;
	Light m_Light;

	bool m_SingleFrameMode;
	bool m_RenderNext;
};