#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

/**
* Singleton class for receiving input from a window
*/
class Input
{
public:
	// Disable copy/move
	Input(const Input&) = delete;
	Input(Input&&) = delete;
	Input& operator=(const Input&) = delete;
	Input& operator=(Input&&) = delete;

	inline static void InitWindow(GLFWwindow* window) {
		if (!Instance().m_Window)
			Instance().m_Window = window;
	};

	enum class Key : int;
	inline static bool IsKeyPressed(Key key) { return glfwGetKey(Instance().m_Window, (int)key); };

	enum class Button : int;
	inline static bool IsMousePressed(Button button) { return glfwGetMouseButton(Instance().m_Window, (int)button); };

	inline static void GetCursor(glm::dvec2& position) { glfwGetCursorPos(Instance().m_Window, &position.x, &position.y); };

private:
	// Don't allow public instantiation
	Input() = default;
	~Input() = default;

	static Input& Instance()
	{
		static Input i;
		return i;
	}

private:
	GLFWwindow* m_Window;
};

enum class Input::Key : int 
{
	A = GLFW_KEY_A,
	B = GLFW_KEY_B,
	C = GLFW_KEY_C,
	D = GLFW_KEY_D,
	E = GLFW_KEY_E,
	F = GLFW_KEY_F,
	G = GLFW_KEY_G,
	H = GLFW_KEY_H,
	I = GLFW_KEY_I,
	J = GLFW_KEY_J,
	K = GLFW_KEY_K,
	L = GLFW_KEY_L,
	M = GLFW_KEY_M,
	N = GLFW_KEY_N,
	O = GLFW_KEY_O,
	P = GLFW_KEY_P,
	Q = GLFW_KEY_Q,
	R = GLFW_KEY_R,
	S = GLFW_KEY_S,
	T = GLFW_KEY_T,
	U = GLFW_KEY_U,
	V = GLFW_KEY_V,
	W = GLFW_KEY_W,
	X = GLFW_KEY_X,
	Y = GLFW_KEY_Y,
	Z = GLFW_KEY_Z,
	SPACE = GLFW_KEY_SPACE,
	L_SHIFT = GLFW_KEY_LEFT_SHIFT,
	R_SHIFT = GLFW_KEY_RIGHT_SHIFT,
	L_ALT = GLFW_KEY_LEFT_ALT,
	R_ALT = GLFW_KEY_RIGHT_ALT,
	L_CTRL = GLFW_KEY_LEFT_CONTROL,
	R_CTRL = GLFW_KEY_RIGHT_CONTROL,
	ENTER = GLFW_KEY_ENTER,
	ESC = GLFW_KEY_ESCAPE,
	// Add more as needed
};

enum class Input::Button : int
{
	LEFT = GLFW_MOUSE_BUTTON_LEFT,
	RIGHT = GLFW_MOUSE_BUTTON_RIGHT,
	MIDDLE = GLFW_MOUSE_BUTTON_MIDDLE,
	// Add more as needed
};

