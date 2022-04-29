#pragma once
#include <vector>

#include "scene.h"
#include "lab1_scene.h"
#include "lab2_scene.h"

#include "test_model.h"

struct Fragment
{
	int x;
	int y;
	float z_inv; // Wonder why 1/z?  https://developer.nvidia.com/content/depth-precision-visualized
	glm::vec3 world_pos;
	glm::vec3 color;
	glm::vec3 world_normal;
};

struct Vertex 
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec3 color;
};

#define TRIANGLE_VERTEX_COUNT 3

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

/**
* A rasterizer description
*	- Describes how geometry should be rasterized
*		- Direct3D: https://docs.microsoft.com/en-us/windows/win32/api/d3d11/ns-d3d11-d3d11_rasterizer_desc
*		- Vulkan: https://www.khronos.org/registry/vulkan/specs/1.2/html/chap25.html#VkPipelineRasterizationStateCreateInfo
*/
struct RasterizerDesc 
{
	FillMode fill_mode;
	CullMode cull_mode;
};

class Lab3Scene : public Scene {
public:
	Lab3Scene();

	/**
	* Draws a triangle using rasterization
	*	- Clears the depth and color buffers
	*	- Invokes DrawTriangle for each triangle in m_Model
	* @param window The window into which a triangle is drawn
	*/
	void Draw(Window& window) override;

	/**
	* Updates scene properties
	*	- Moves the m_Camera based on user Input.
	*	- Moves the m_Light based on user Input.
	* @param dt The delta time in seconds since this Update-method was called last.
	*/
	void Update(float dt) override;

	/**
	* Updates GUI properties
	*	-
	*/
	void DrawGUI() override;

	inline const char* GetName() override { return "Lab-3-Rasterization"; };

private:

	/**
	* Linearly interpolates two Pixels a and b by the size of result such that.
	*	- The first element of result is a
	*	- The last element of result is b
	*	- All values of result are equidistant and linearly dependent on a and b.
	*	- All properties of the Fragments are interpolated. (see Fragment struct)
	*	- Special case: If 'result' has size 1, take the average of a and b.
	* @param a The Pixel to be interpolated from
	* @param b The Pixel to be interpolated to
	* @param result A list of size n to be populated with n linearly interpolated Pixels including and between a and b
	*/
	void Interpolate(const Fragment& a, const Fragment& b, std::vector<Fragment>& result);

	/**
	* Draws a line to a Window starting from fragment 'start' and ending on fragment 'end'.
	*	- Calculates the maximum number of pixels between and including start and end
	*	- Interpolates between start and end creating a list of all fragments in the line
	*	- Draws each fragment by invoking a FragmentShader
	* @param window The window into which a triangle is drawn
	* @param start The first fragment to draw in the line going to 'end'
	* @param end The final fragment to draw in the line coming from 'start'
	*/
	void DrawLine(Window& window, const Fragment& start, const Fragment& end);

	/**
	* Draws a triangle consisting of three vertices into a Window based on a rasterizer description.
	*	- The rasterizer description determines whether to fill the triangle and if the triangle should be culled. 
	*	- Mimics a very bare-bones rendering pipeline: https://www.khronos.org/opengl/wiki/Rendering_Pipeline_Overview
	*	- Runs following pipeline stages in order:
	*		- Vertex shader
	*		- Primitive assembly
	*		- Rasterization
	*			- Fragment shader
	* @param window The window into which a triangle is drawn
	* @param rasterizer_desc A description stating how the triangle should be rasterized
	* @param triangle A set of three vertices to be rasterized into a triangle
	*/
	void DrawTriangle(Window& window, const RasterizerDesc& rasterizer_desc, Vertex triangle[TRIANGLE_VERTEX_COUNT]);

	/**
	* A Vertex Shader 
	*	- Transforms a vertex in world space (x, y, z) into a point in screen space (u, v). 
	*	- (u, v) is inside the output image bounds if u in [0, width) and v in [0, height).
	*	- The output is called a fragment, and represents a pixel with some additional attributes in the drawn image.
	*	- Vertex shader output is passed to the rasterizer
	* Example:
	*	- GLSL: https://www.khronos.org/opengl/wiki/Vertex_Shader
	* @param vs_in The vertex to transform
	* @param vs_out The pixel information to produce
	* @param window The window to be used to transform positions to clip-space
	*/
	void VertexShader(const Vertex& vs_in, Fragment& vs_out, const Window& window);

	/**
	* A Primitive Assembler
	*	- "Assembles" the triangle and performs culling based on rasterizer_desc.cull_mode
	* @param rasterizer_desc A description stating how the triangle should be rasterized
	* @param vs_out The three vertex shader outputs corresponding to the edge fragments of a triangle
	* @returns true if the triangle is visible and should be passed to rasterization, false otherwise.
	*/
	bool PrimitiveAssembly(const RasterizerDesc& rasterizer_desc, const Fragment vs_out[TRIANGLE_VERTEX_COUNT]);

	/**
	* The rasterization stage (generally fixed function / non programmable in practice)
	*	- Takes three fragments output by the vertex shader and interpolates fragments inbetween.
	*	- Depending on rasterizer_desc, the triangle is rasterized as filled or drawn in wireframe.
	*	- Invokes FragmentShader for each interpolated fragment.
	* Example:
	*	- OpenGL: https://www.khronos.org/opengl/wiki/Rasterization
	*	- Vulkan: https://vulkan-tutorial.com/Drawing_a_triangle/Graphics_pipeline_basics/Fixed_functions#page_Rasterizer
	* @param window The window into which fragments are rasterized
	* @param rasterizer_desc A description stating how the triangle should be rasterized
	* @param vs_out The three vertex shader outputs corresponding to the edge fragments of a triangle
	*/
	void Rasterization(Window& window, const RasterizerDesc& rasterizer_desc, Fragment vs_out[TRIANGLE_VERTEX_COUNT]);
	
	/**
	* A Fragment Shader
	*	- Takes a fragment as input from the rasterization stage.
	*	- Calculates lighting based on m_Camera, m_Light and the fragment's properties
	*	- Draws color to the Window at the fragment's position
	*	- Only draws if this fragment is closer than previously drawn fragments,
	*		which depends on the value stored in the m_DepthBuffer for the fragment's position.
	* Example:
	*	- OpenGL: https://www.khronos.org/opengl/wiki/Fragment_Shader
	* @param window The window into which shaded fragments are drawn
	* @param in The input fragment coming from the rasterizer to be shaded
	*/
	void FragmentShader(Window& window, const Fragment& in);

private:
	std::vector<float> m_DepthBuffer;
	std::vector<Triangle> m_Model;
	Camera m_Camera;
	Light m_Light;

	RasterizerDesc m_RasterizerDesc;
};