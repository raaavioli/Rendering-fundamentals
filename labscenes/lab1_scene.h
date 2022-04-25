#pragma once
#include <vector>

#include "scene.h"


class Lab1Scene : public Scene {
public:
	Lab1Scene() : m_DrawColors(true) { InitStarfield(1000); }

	void Draw(Window& window) override;

	/**
	* Colors: Nothing
	* 
	* Starfield:
	* Updates the position p_t at time t of all stars such that they move away from the camera, while the camera is pointing 
	* in the direction of the z-axis. Each particle position p_t is updated as follows:
	*	p_{t}.x = p_{t-1}.x
	*	p_{t}.y = p_{t-1}.y
	*	p_{t}.z = p_{t-1}.z - v * dt
	* where v is the speed of the particle in direction z.
	* 
	* The requirement for a particle's position stated in InitStarfield should be kept here as well.
	* If a particle moves outside of its boundary, it wraps around to the start again.
	* 
	* @param dt The delta time in seconds, giving the time since this Update-method was called last.
	*/
	void Update(float dt) override;

	/**
	* Colors:
	*	- Add glm::vec3 members m_TopLeft, m_TopRight, m_BottomLeft, m_BottomRight
	*	- Use ImGui::ColorEdit3 to allow manipulation of color members
	*	- Apply colors by invoking DrawColors with appropriate arguments.
	* 
	* Starfield: 
	*	- Add member float m_Speed, to dynamically change the speed of the animation using ImGui::SliderFloat
	*	- Add member glm::vec3 m_StarColor to dynamically change the initial star color using ImGui::ColorEdit3
	*	- Add member float m_FocalLength to dynamically control the focal length of the camera going between 0 and H.
	*/
	void DrawGUI() override;

	inline const char* GetName() override { return "Lab-1"; };

private:
	/**
	* Linearly interpolates a and b by the size of result such that.
	*	- The first element of result is a
	*	- The last element of result is b
	*	- All values inbetween are equidistant and linearly dependent on a and b.
	* @param a The value to be interpolated from
	* @param b The value to be interpolated to
	* @param result A list of size n to be populated with n linearly interpolated values including and between a and b
	*/
	template<typename T>
	void Interpolate(T a, T b, std::vector<T>& result);

	/**
	* Bilinearly interpolates colors top_left, top_right, bottom_left and bottom_right over all pixels of the window.
	* 
	* @param window The window to be drawn into
	* @param top_left The color to appear in the top left corner of the window
	* @param top_right The color to appear in the top right corner of the window
	* @param bottom_left The color to appear in the bottom left corner of the window
	* @param bottom_right The color to appear in the bottom right corner of the window
	*/
	void DrawColors(Window& window, glm::vec3 top_left, glm::vec3 top_right, glm::vec3 bottom_left, glm::vec3 bottom_right);
	
	/**
	* Initializes positions of all stars in m_Stars
	*	- Initialize star positions in m_Stars such that:
	*	1. -1 <= x <= 1
	*	2. -1 <= y <= 1
	*	3.  0 <  z <= 1
	*	- Tip: Generate random values between 0 and 1 with float(rand()) / float(RAND_MAX).
	* @param n The number of stars to be initialized
	*/
	void InitStarfield(uint32_t n);

	/**
	* Draws the starfield in m_Stars using a pinhole camera projection from 3D world space to 2D screen space.
	* The camera is positioned at the origin (0, 0, 0) and is looking in the negative z-direction.
	* The center of the world space (0, 0, 0) is projected to the center of the screen (W/2, H/2), 
	* for a screen with width W and height H.
	*	- The projection from a world position (x, y, z) to a screen position (u, v) uses the following formula:
	*		u = f * x / z + W / 2
	*		v = f * y / z + H / 2
	*	where f is the focal length of the camera. f = H/2 makes the vertical field of view 90 degrees.
	* 
	* Each star is projected and drawn onto the screen using its projected screen coordinates.
	* 
	* @param window The window to draw into
	* @param W The width of the pinhole projection
	* @param H The height of the pinhole projection
	* @param f The focal length of the camera
	* @param star_color The initial color of the stars when they are as close as possible to the camera.
	*/
	void DrawStarfield(Window& window, float W, float H, float f, glm::vec3 star_color);

private:
	bool m_DrawColors;

	std::vector<glm::vec3> m_Stars;
};