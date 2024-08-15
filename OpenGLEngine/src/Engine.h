#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include <iostream>

#include "Camera.h"
#include "Renderer.h"

class Engine {
private:
	float m_Width = 800;
	float m_Height = 600;

	GLFWwindow* m_Window;
	Camera camera = Camera(m_Width, m_Height, 1.0f, 100.0f);
	Renderer renderer = Renderer();

public:
	Engine(float width, float height);
	~Engine();

	void run();
private:
	void setupGLFW();
	void setupOpenGL();
	void setupImGUI();

	void drawImGUI();
	void clearImGUI();

	// Callback functions

	void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

};