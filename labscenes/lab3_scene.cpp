#include "lab3_scene.h"

#include <input.h>

Lab3Scene::Lab3Scene()
{
	LoadTestModel(m_Model);
	m_Camera.position = glm::vec3(0.0, 0.0, -2.0);
	m_Camera.yaw = 0.0f;
	m_Light.position = glm::vec3(0.0f, 0.0f, 0.0);
	m_Light.color = glm::vec3(1.f, 1.f, 1.f);
	m_RasterizerDesc.cullMode = CullMode::NONE;
	m_RasterizerDesc.fillMode = FillMode::SOLID;
}

void Lab3Scene::Draw(Window& window)
{;
	if (m_DepthBuffer.size() != (window.GetWidth() * window.GetHeight()))
		m_DepthBuffer.resize(window.GetWidth() * window.GetHeight());

	// Clear window and depth buffer
	window.Clear(glm::vec3(0));
	for (int y = 0; y < window.GetHeight(); y++)
		for (int x = 0; x < window.GetWidth(); x++)
			m_DepthBuffer[y * window.GetWidth() + x] = 0.0f;

	// TODO: To be implemented in Lab 3.x.x, see spec in lab1_scene.h

}

void Lab3Scene::Update(float dt)
{
	// TODO: To be implemented in Lab 3.x.x, see spec in lab1_scene.h
}

void Lab3Scene::DrawGUI()
{
	static bool wireframe = false;
	if (ImGui::Checkbox("Wireframe", &wireframe))
		m_RasterizerDesc.fillMode = wireframe ? FillMode::WIREFRAME : FillMode::SOLID;

	// TODO: To be implemented in Lab 3.x.x, see spec in lab1_scene.h
}

void Lab3Scene::Interpolate(glm::ivec2 a, glm::ivec2 b, std::vector<glm::ivec2>& result)
{
	// TODO: To be implemented in Lab 3.x.x, see spec in lab1_scene.h
}

void Lab3Scene::Interpolate(const Pixel& a, const Pixel& b, std::vector<Pixel>& result)
{
	// TODO: To be implemented in Lab 3.x.x, see spec in lab1_scene.h
}

void Lab3Scene::DrawLine(Window& window, glm::ivec2 start, glm::ivec2 end, glm::vec3 color)
{
	// TODO: To be implemented in Lab 3.x.x, see spec in lab1_scene.h
}

void Lab3Scene::DrawPolygon(Window& window, const RasterizerDesc& rasterizerDesc, const std::vector<Vertex>& vertices)
{
	// TODO: To be implemented in Lab 3.x.x, see spec in lab1_scene.h
}

void Lab3Scene::VertexShader(Window& window, const Vertex& in, Pixel& out)
{
	// TODO: To be implemented in Lab 3.x.x, see spec in lab1_scene.h
}

void Lab3Scene::FragmentShader(Window& window, const Pixel& in)
{
	// TODO: To be implemented in Lab 3.x.x, see spec in lab1_scene.h
}
