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

	/**
	* Returns the rotation
	*/
	glm::mat3 GetRotationY() const;
};

#define PI 3.1415926535f

class Lab2Scene : public Scene {
public:
	Lab2Scene();

	void Draw(Window& window) override;

	/**
	* Updates the m_Camera by polling Input.
	*
	* @param dt The delta time in seconds, giving the time since this Update-method was called last.
	*/
	void Update(float dt) override;

	void DrawGUI() override;

	inline const char* GetName() override { return "Lab-2-Raytracing"; };

private:
	/**
	* Finds closest intersection of ray among triangles.
	* - Any point p in a triangle is given by: 
	*		p = v0 + ue1 + ve2, such that:
	*		e1 and e2 are edge vectors originating at vertex v0, pointing towards v1 and v2 respectively
	*		0	<= u
	*		0	<= v
	*		u+v	<= 1
	* - Any point p along a ray is given by:
	*		p = s + td, such that:
	*		s is the starting point of the ray.
	*		t is the direction of the ray
	*		0	<  t, since we are only interested in triangles in positive d-direction.
	* - We solve for coordinates (t, u, v) where:
	*		v0 + ue1 + ve2 = s + td
	* - We simplify the equation:
	*		-td + ue1 + ve2 = s - v0
	*		(-d e1 e2) * (t u v)^(T) = s - v0
	*	And define 3x3 matrix A = (-d e1 e2), vector x = (t u v)^(T) and vector b = (s - v0).
	* 
	*	Finding the intersection thus implies the equation Ax = b for x.
	*	This achieved through. inverse(A)*b = x.
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
	* - Approximates light intensity (I) with the blinn-phong shading model:
	*	I = ambient * surface_color + (1 - shadow) * (diffuse * surface_color + specular * light_color)
	*	where 
	*	ambient:	The approximate global illumination (0-1)
	*	shadow:		The amount of "shadowness" at the intersection (0-1)
	*	diffuse:	dot(N, L), where N is the normalized normal at intersection, and L is the direction to the light source (0-1)
	*	specular:	dot(R, V)^p or dot(N, H)^p, where R is the reflection vector, V is the view vector, N is the normal and H is L + V. 
					p is an exponent dictating the intensity of the specular reflection. (0-1)
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