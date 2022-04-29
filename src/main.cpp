#include <glm/glm.hpp>

#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>
#include <imgui.h>

#include <helpers.h>
#include <window.h>
#include <scene.h>
#include <input.h>

/* TODO: Include custom scenes here */
#include <example_scene.h>
#include <lab1_scene.h>
#include <lab2_scene.h>
#include <lab3_scene.h>

int main(void)
{
	Scene* scenes[] = {
		new ExampleScene(),
		new Lab1Scene(),
		new Lab2Scene(),
		new Lab3Scene(),
		/* TODO: Add new scenes here */
	};

	int current_scene = 0;
	const int scene_count = sizeof(scenes) / sizeof(scenes[current_scene]);
	const char* scene_names[scene_count] = {0};
	for (int i = 0; i < scene_count; i++)
	{
		scene_names[i] = scenes[i]->GetName();
	}

	glfwSetErrorCallback(ErrorCallback);
	if (!glfwInit())
		exit(EXIT_FAILURE);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	const uint32_t WIDTH = 640;
	const uint32_t HEIGHT = 480;
	GLFWwindow* glfwWindow = glfwCreateWindow(WIDTH, HEIGHT, "DH2323 Rendering", NULL, NULL);
	if (!glfwWindow)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwSetKeyCallback(glfwWindow, KeyCallback);
	glfwMakeContextCurrent(glfwWindow);

	Input::InitWindow(glfwWindow);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cerr << "ERROR: failed to initialize OpenGL context \n" << std::endl;
		exit(EXIT_FAILURE);
	}
	GL_CHECK(glClearColor(1.0, 0.0, 0.0, 1.0));
	glfwSwapInterval(1);

	// Initialize ImGui with docking and viewports
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= (ImGuiConfigFlags_DockingEnable | ImGuiConfigFlags_ViewportsEnable);
	io.ConfigDockingWithShift = false;
	ImGui_ImplGlfw_InitForOpenGL(glfwWindow, true);
	ImGui_ImplOpenGL3_Init(s_GLSLVersion);

	// Allocate window with half resolution backbuffer
	Window appWindow(WIDTH / 2, HEIGHT / 2);
	const glm::vec3 CLEAR_COLOR = glm::vec3(0.0, 0.0, 1.0);
	appWindow.Clear(CLEAR_COLOR);

	uint32_t backbufferTexture = 0;
	ReallocateTexture(&backbufferTexture, appWindow);

	uint32_t backbufferShader = CreateShaderProgram(s_VertexShaderText, s_FragmentShaderText);
	uint32_t textureLocation = glGetUniformLocation(backbufferShader, "u_Texture");

	// Create empty VAO since OpenGL requires a vertex array to be bound to call glDrawArrays
	// However this VAO has no attached buffers, since it is only used to draw 6 vertices 
	// without attributes (fullscreen quad)
	uint32_t emptyVAO = 0;
	if (!emptyVAO)
		GL_CHECK(glGenVertexArrays(1, &emptyVAO));
	GL_CHECK(glBindVertexArray(emptyVAO));

	float time = (float)glfwGetTime();
	while (!glfwWindowShouldClose(glfwWindow))
	{
		int width, height;
		glfwGetFramebufferSize(glfwWindow, &width, &height);
		GL_CHECK(glViewport(0, 0, width, height));
		GL_CHECK(glClear(GL_COLOR_BUFFER_BIT));

		// Update
		float current_time = (float)glfwGetTime();
		float dt = current_time - time;
		time = current_time;
		scenes[current_scene]->Update(dt);

		// Draw scene to Backbuffer
		scenes[current_scene]->Draw(appWindow);

		// Draw Backbuffer to default framebuffer (Screen)
		// Drawing fullscreen quad with 2 triangles having 3 vertices each. See s_VertexShaderText in helpers.h.
		glUseProgram(backbufferShader);
		const uint32_t activeTexture = 0;
		GL_CHECK(glActiveTexture(GL_TEXTURE0 + activeTexture));
		GL_CHECK(glBindTexture(GL_TEXTURE_2D, backbufferTexture));
		GL_CHECK(glTexSubImage2D(
			GL_TEXTURE_2D,              // target
			0,                          // level
			0,                          // xoffset
			0,                          // yoffset
			appWindow.GetWidth(),
			appWindow.GetHeight(),      // width, height
			GL_RGB,                     // image format
			GL_FLOAT,                   // pixel data type
			appWindow.GetBufferPtr()    // data
		));
		GL_CHECK(glBindVertexArray(emptyVAO));
		GL_CHECK(glUniform1i(textureLocation, activeTexture));
		GL_CHECK(glDrawArrays(GL_TRIANGLES, 0, 6));
		GL_CHECK(glBindVertexArray(0));

		// Draw ImGui
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		static ImGuiID dockspaceID = 0;
		bool active = true;
		if (ImGui::Begin("Settings", &active))
		{
			ImGui::Spacing();

			// Backbuffer settings
			ImGui::Text("Backbuffer: %dx%d", appWindow.GetWidth(), appWindow.GetHeight());
			static int dims[2] = { (int)appWindow.GetWidth(), (int)appWindow.GetHeight() };
			ImGui::SameLine(); ImGui::SliderInt2("", dims, 128, 1024);
			ImGui::SameLine();
			if (ImGui::Button("Resize"))
			{
				appWindow.Resize(dims[0], dims[1]);
				ReallocateTexture(&backbufferTexture, appWindow);
			}

			// Performance metrics
			ImGui::Text("FPS: %f, Time: %f (ms)", 1 / dt, dt * 1000.0f);
			ImGui::Spacing();

			// Taking screenshots
			static char screenshot_name[40] = "lab_screenshot";
			ImGui::InputText(" ", screenshot_name, sizeof(screenshot_name));
			ImGui::SameLine();
			if (ImGui::Button("Take screenshot"))
				appWindow.TakeScreenshot(screenshot_name);

			ImGui::Spacing();

			// Scene selection
			static const char* current_item = scene_names[0];
			if (ImGui::BeginCombo("##combo", current_item))
			{
				for (int i = 0; i < IM_ARRAYSIZE(scene_names); i++)
				{
					bool is_selected = (current_item == scene_names[i]);
					if (ImGui::Selectable(scene_names[i], is_selected))
					{
						current_item = scene_names[i];
						current_scene = i;
						appWindow.Clear(CLEAR_COLOR);
					}
					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}
			ImGui::Spacing();

		}
		if (active)
		{
			dockspaceID = ImGui::GetID("Main_DockSpace");
			ImGui::DockSpace(dockspaceID, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None | ImGuiDockNodeFlags_PassthruCentralNode);
		}
		ImGui::End();

		// Draw scene specific GUI panel
		ImGui::SetNextWindowDockID(dockspaceID, ImGuiCond_FirstUseEver);
		if (ImGui::Begin("Scene settings"))
		{
			scenes[current_scene]->DrawGUI();
		}
		ImGui::End();

		ImGui::EndFrame();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		GLFWwindow* context = glfwGetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(context);

		glfwSwapBuffers(glfwWindow);
		glfwPollEvents();
	}

	for (int i = 0; i < scene_count; i++)
		delete scenes[i];

	glDeleteVertexArrays(1, &emptyVAO);
	glDeleteProgram(backbufferShader);
	glDeleteTextures(1, &backbufferTexture);

	glfwDestroyWindow(glfwWindow);

	ImGui::DestroyPlatformWindows();
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();
	exit(EXIT_SUCCESS);
}