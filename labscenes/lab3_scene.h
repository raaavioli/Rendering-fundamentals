#pragma once
#include <vector>

#include "scene.h"
#include "lab1_scene.h"
#include "lab2_scene.h"

#include "test_model.h"

struct Pixel 
{
	int x;
	int y;
	float z_inv;
	glm::vec3 position;
	glm::vec3 color;
	glm::vec3 normal;
};

struct Vertex 
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec3 color;
};

enum class FillMode : uint32_t 
{
	WIREFRAME	= 2,
	SOLID		= 3,
};

enum class CullMode : uint32_t
{
	NONE	= 0,
	FRONT	= 1,
	BACK	= 2,
};

struct RasterizerDesc 
{
	FillMode fillMode;
	CullMode cullMode;
};

class Lab3Scene : public Scene {
public:
	Lab3Scene();

	void Draw(Window& window) override;

	void Update(float dt) override;

	void DrawGUI() override;

	inline const char* GetName() override { return "Lab-3-Rasterization"; };

private:
	void Interpolate(glm::ivec2 a, glm::ivec2 b, std::vector<glm::ivec2>& result);
	void Interpolate(const Pixel& a, const Pixel& b, std::vector<Pixel>& result);
	void DrawLine(Window& window, glm::ivec2 start, glm::ivec2 end, glm::vec3 color);
	void DrawPolygon(Window& window, const RasterizerDesc& rasterizerDesc, const std::vector<Vertex>& vertices);
	void VertexShader(Window& window, const Vertex& in, Pixel& out);
	void FragmentShader(Window& window, const Pixel& in);

private:
	std::vector<Triangle> m_Model;
	Camera m_Camera;
	Light m_Light;

	std::vector<float> m_DepthBuffer;

	RasterizerDesc m_RasterizerDesc;
};