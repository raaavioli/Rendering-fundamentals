#include "lab3_scene.h"

#include <input.h>

Lab3Scene::Lab3Scene()
{
	LoadTestModel(m_Model);
	m_Camera.position = glm::vec3(0.0, 0.0, -2.0);
	m_Camera.yaw = 0.0f;
	m_Light.position = glm::vec3(0.0f, 0.0f, 0.0);
	m_Light.color = glm::vec3(1.f, 1.f, 1.f);
	m_RasterizerDesc.cull_mode = CullMode::NONE;
	m_RasterizerDesc.fill_mode = FillMode::SOLID;
}

void Lab3Scene::Draw(Window& window)
{
	// Resize depth buffer if window resolution changed.
	const uint32_t WIDTH = window.GetWidth();
	const uint32_t HEIGHT = window.GetHeight();
	if (m_DepthBuffer.size() != (WIDTH * HEIGHT))
		m_DepthBuffer.resize(WIDTH * HEIGHT);

	// Clear window and depth buffer
	window.Clear(glm::vec3(0));
	for (uint32_t y = 0; y < HEIGHT; y++)
		for (uint32_t x = 0; x < WIDTH; x++)
			m_DepthBuffer[y * WIDTH + x] = 0.0f;

	// TODO: To be implemented in Lab 3.1.1, see spec in lab3_scene.h
}

void Lab3Scene::Update(float dt)
{
	// TODO: To be implemented in Lab 3.x.x, see spec in lab3_scene.h
}

void Lab3Scene::DrawGUI()
{
	static bool wireframe = false;
	if (ImGui::Checkbox("Wireframe", &wireframe))
		m_RasterizerDesc.fill_mode = wireframe ? FillMode::WIREFRAME : FillMode::SOLID;

	static const char* cull_mode_names[] = {"None", "Frontface", "Backface"};
	static const char* current_item = cull_mode_names[0];
	if (ImGui::BeginCombo("##cullmode", current_item))
	{
		for (int i = 0; i < IM_ARRAYSIZE(cull_mode_names); i++)
		{
			bool is_selected = (current_item == cull_mode_names[i]);
			if (ImGui::Selectable(cull_mode_names[i], is_selected))
			{
				current_item = cull_mode_names[i];
				m_RasterizerDesc.cull_mode = (CullMode)i;
			}
			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}
	ImGui::SameLine(); ImGui::Text("Culling");

	// Add GUI elements as needed
}


void Lab3Scene::Interpolate(const Fragment& a, const Fragment& b, std::vector<Fragment>& result)
{
	// TODO: To be implemented in Lab 3.1.5, see spec in lab3_scene.h
}

void Lab3Scene::DrawLine(Window& window, const Fragment& start, const Fragment& end)
{
	// TODO: To be implemented in Lab 3.1.4, see spec in lab3_scene.h
}

void Lab3Scene::DrawTriangle(Window& window, const RasterizerDesc& rasterizer_desc, Vertex vertices[TRIANGLE_VERTEX_COUNT])
{
	// TODO: To be implemented in Lab 3.1.2, see spec in lab3_scene.h
}

void Lab3Scene::VertexShader(const Vertex& vs_in, Fragment& vs_out, const Window& window)
{
	// TODO: To be implemented in Lab 3.1.3, see spec in lab3_scene.h
	// Tips:
	//	- Translate vertex to camera space (P - C). (center world around the camera) 
	//	- Rotate vertex in camera space by camera's rotation. (P - C)*inv(R) 
	//		Notice: Rotating camera to left <=> Rotating the world to right by the same angle
	//	- Given focal length f, screen width W and height H, projecting a point to screen space (X, Y, Z) -> (x, y)
	//		Can be written as:					(x, y) = (X, Y) * f / Z.
	//		Origin around center of screen:		(u, v) = (x, y) + (W, W) / 2.0f
	//	- vs_out.z_inv = 1/z, Wonder why? -> https://developer.nvidia.com/content/depth-precision-visualized
}

bool Lab3Scene::PrimitiveAssembly(const RasterizerDesc& rasterizer_desc, const Fragment vs_out[TRIANGLE_VERTEX_COUNT])
{
	// Optional: Implement backface culling based on raterizer_desc.cull_mode
	switch (m_RasterizerDesc.cull_mode)
	{
	case CullMode::BACK:
		// Cull if triangle is facing away from the camera
		break;
	case CullMode::FRONT:
		// Cull if triangle is facing the camera
		break;
	case CullMode::NONE:
		// Do nothing, draw both sides
		break;
	}
	return true;
}

void Lab3Scene::Rasterization(Window& window, const RasterizerDesc& rasterizer_desc, Fragment vs_out[TRIANGLE_VERTEX_COUNT])
{	
	switch (m_RasterizerDesc.fill_mode)
	{
	case FillMode::WIREFRAME:
		// TODO: To be implemented in Lab 3.1.4, see spec in lab3_scene.h
		// Wireframe rasterization creating the outline of the triangle
		// Tips: Use DrawLine
		break;

	case FillMode::SOLID:
		// TODO: To be implemented in Lab 3.2.1, see spec in lab3_scene.h
		// Solid rasterization creating a filled triangle surface.

		// 1. Find max and min y-value of the triangle.
		//	- This gives the number of pixel rows in the image the triangle occupies.
		//	- The number of rows is given by max_y - min_y + 1;
		//	- You can use (+/-) std::numeric_limits<int>::max() to initialize min/max.

		int polygon_rows /*= max_y - min_y + 1*/;

		// 2. Create two lists with the left and right-most fragments covered by the triangle:
		//	Example:
		// Rows:
		// 7              lr
		// 8            l   r
		// 9          l      r
		// 10       l         r
		// 11          l       r
		// 12             l     r
		// 13                l   r 
		// 14                   l-r
		// Where l represents the leftmost fragments, and r represents the right-most fragments 
		// in the triangle. This triangle has polygon_rows = 14 - 7 + 1 = 8.
		std::vector<Fragment> left_fragments(polygon_rows);
		std::vector<Fragment> right_fragments(polygon_rows);

		// 2.1. Initialize x-coordinates in left_fragments and right_fragments
		// to extreme values.


		// 2.2. Loop over all edges of the triangle. 
		//	- Create a list to contain all fragments along the edge
		//	- Use Interpolate to calculate coordinates for each fragment along the edge.
		//	- For each fragment along the edge, populate left_fragments and right_fragments
		//		with the fragments furthest left/right for each row. 
		// Tips: Be careful not to read/write out of bounds of left_fragments/right_fragments.


		// 3.0. For each row, Interpolate between left_fragments and right_fragments.
		//	- FragmentShader is invoked for each interpolated fragment, which gives the triangle
		//	a filled/solid surface.

		break;
	}
}

void Lab3Scene::FragmentShader(Window& window, const Fragment& in)
{
	// TODO: To be implemented in Lab 3.1.6, see spec in lab3_scene.h
	// Tips: 
	//	- Check if this fragment is closer than previously drawn fragments at the same location using m_DepthBuffer
	//	- You can reuse the blinn-phong shading model created in Lab2, but feel free to experiment.
	//	- Wireframe can appear washed out when shaded. 
	//		Create a way to toggle flat color shading using ImGui.
}
