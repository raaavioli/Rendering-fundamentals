#include "example_scene.h"

#define PI 3.14159265f

void ExampleScene::Update(float dt)
{
	static float total_time = 0;
	total_time += dt;
	m_Sphere.position.x = glm::sin(total_time / 2.0f * PI) / 2.0f;
	m_Sphere.position.y = glm::cos(total_time / 5.0f * PI) / 2.0f;
}

void ExampleScene::Draw(Window& window)
{
	for (uint32_t y = 0; y < window.GetHeight(); y++)
	{
		for (uint32_t x = 0; x < window.GetWidth(); x++)
		{
			glm::vec2 UV = glm::vec2(x, y) / glm::vec2(window.GetWidth() - 1, window.GetHeight() - 1);
			glm::vec3 color0 = GetSphereColor(glm::vec2(UV.x, UV.y));
			window.PutPixel(x, y, color0);
		}
	}
}

void ExampleScene::DrawGUI()
{
	ImGui::DragFloat2("Light position", (float*) &m_LightPos, 0.1f, -10, 10);
}

glm::vec3 ExampleScene::GetSphereColor(glm::vec2 UV)
{
	glm::vec3 ambient(0.01f, 0.05f, 0.08f);
	glm::vec2 centeredUV = UV * 2.0f - 1.0f;

	glm::vec2 fragment_dir = centeredUV - m_Sphere.position;
	float dist_to_center = glm::length(fragment_dir) / m_Sphere.radius;
	if (dist_to_center < 1.0f)
	{
		float angle = glm::acos(dist_to_center);
		float z = glm::sin(angle) * m_Sphere.radius;
		glm::vec3 normal = normalize(glm::vec3(fragment_dir, z));
		glm::vec3 surface_pos = glm::vec3(m_Sphere.position, 0.0f) + normal * m_Sphere.radius;
		glm::vec3 light_dir = glm::normalize(m_LightPos - surface_pos);
		glm::vec3 camera_pos = glm::vec3(0.0, 0.0, 1.0f);
		glm::vec3 camera_dir = glm::normalize(camera_pos - surface_pos);

		float lambert = glm::max(glm::dot(normal, light_dir), 0.0f);
		glm::vec3 half_way = glm::normalize(light_dir + camera_dir);
		float spec = glm::pow(glm::dot(half_way, normal), 20.0f) * (lambert > 0);

		glm::vec3 diffuse = lambert * glm::vec3(1, 0, 0);
		float specular_intensity = 0.6f;
		glm::vec3 specular = spec * glm::vec3(1, 1, 1) * specular_intensity;
		return glm::max(diffuse + specular, ambient);
	}
	return ambient;
}
