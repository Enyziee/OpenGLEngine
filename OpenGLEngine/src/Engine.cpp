#include "Engine.h"

#include <rapidobj/rapidobj.hpp>

#include "Object.h"
#include "ShaderProgram.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <functional>

Engine::Engine(float width, float height) {
	m_Width = width;
	m_Height = height;

	setupGLFW();
	setupOpenGL();
	setupImGUI();

	this->camera = Camera(width, width, 1.0f, 100.0f);
}

Engine::~Engine() {

	glfwTerminate();
	return;
}

void Engine::run() {

	//auto cube = Object("./res/objects/teapot.obj");
	auto teapot = rapidobj::ParseFile("./res/objects/teapot.obj");
	std::vector<unsigned int>* bufferIndices = new std::vector<unsigned int>;

	for (size_t i = 0; i < teapot.shapes.size(); i++) {
		auto& object = teapot.shapes.at(i);

		for (size_t j = 0; j < object.mesh.indices.size(); j++) {
			auto value = object.mesh.indices[j].position_index;
			bufferIndices->push_back(value);
		}
	}




	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * teapot.shapes[0].mesh.indices.size(), teapot.shapes[0].mesh.indices.data(), GL_STATIC_DRAW);

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * bufferIndices->size(), bufferIndices->data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	ShaderProgram shader = ShaderProgram("./res/shader/default");
	shader.bind();

	camera.setPosition({0.0f, 0.0f, 3.0f});


	while (!glfwWindowShouldClose(m_Window)) {

		this->drawImGUI();

		

		//shader.setUniformMatrix4fv("u_Model", glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.0f)));
		//shader.setUniformMatrix4fv("u_ViewProjection", camera.getViewProjectionMatrix());


		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBindVertexArray(VAO);
		
		glDrawElements(GL_TRIANGLES, bufferIndices->size(), GL_UNSIGNED_INT, 0);

		//renderer.draw(cube.getVertexArray(), cube.getIndexBuffer());
		

		renderer.clear();
		this->clearImGUI();

		glfwSwapBuffers(m_Window);
		glfwPollEvents();
	}
}

void Engine::setupGLFW() {

	if (!glfwInit()) {
		std::cout << "Failed to initialize GLFW" << std::endl;
		return;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
	//glfwWindowHint(GLFW_RESIZABLE, false);

	m_Window = glfwCreateWindow(m_Width, m_Height, "Hello World", NULL, NULL);
	if (!m_Window) {
		std::cout << "Failed to create GLFW Window" << std::endl;
		glfwTerminate();
		return;
	}

	glfwMakeContextCurrent(m_Window);
	glfwSwapInterval(1);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return;
	}


	glfwSetFramebufferSizeCallback(m_Window, [](GLFWwindow* window, int width, int height) {
		std::cout << "Window resized: " << width << "x" << height << std::endl;
	});

		
	
}

void APIENTRY glDebugOutput(GLenum source,
	GLenum type,
	unsigned int id,
	GLenum severity,
	GLsizei length,
	const char* message,
	const void* userParam) {
	// ignore non-significant error/warning codes
	if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

	std::cout << "---------------" << std::endl;
	std::cout << "Debug message (" << id << "): " << message << std::endl;

	switch (source) {
	case GL_DEBUG_SOURCE_API:             std::cout << "Source: API"; break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Source: Window System"; break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break;
	case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Source: Third Party"; break;
	case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Source: Application"; break;
	case GL_DEBUG_SOURCE_OTHER:           std::cout << "Source: Other"; break;
	} std::cout << std::endl;

	switch (type) {
	case GL_DEBUG_TYPE_ERROR:               std::cout << "Type: Error"; break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour"; break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Type: Undefined Behaviour"; break;
	case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Type: Portability"; break;
	case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Performance"; break;
	case GL_DEBUG_TYPE_MARKER:              std::cout << "Type: Marker"; break;
	case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Type: Push Group"; break;
	case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Type: Pop Group"; break;
	case GL_DEBUG_TYPE_OTHER:               std::cout << "Type: Other"; break;
	} std::cout << std::endl;

	switch (severity) {
	case GL_DEBUG_SEVERITY_HIGH:         std::cout << "Severity: high"; break;
	case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "Severity: medium"; break;
	case GL_DEBUG_SEVERITY_LOW:          std::cout << "Severity: low"; break;
	case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: notification"; break;
	} std::cout << std::endl;
	std::cout << std::endl;
}


void Engine::setupOpenGL() {
	int flags = 0;
	glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
	if (flags & GL_CONTEXT_FLAG_DEBUG_BIT) {
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(glDebugOutput, nullptr);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
	}

	glEnable(GL_DEPTH_TEST);
	glViewport(0, 0, m_Width, m_Height);

}

void Engine::setupImGUI() {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad 
	ImGui::StyleColorsDark();

	ImGui_ImplGlfw_InitForOpenGL(m_Window, true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
	ImGui_ImplOpenGL3_Init();
}

void Engine::drawImGUI() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	static float f = 0.0f;
	static int counter = 0;

	ImGui::Begin("Info");
	
	
	ImGui::Text("Camera Position (%.2f %.2f %.2f)", camera.getPosition().x, camera.getPosition().y, camera.getPosition().z);
	ImGui::Text("Camera Rotation (%.2f %.2f %.2f)", camera.getDirection().x, camera.getDirection().y, camera.getDirection().z);

	float pos[3]{};

	ImGui::SliderFloat3("Position", pos, 0, 10, "%.2f");
}

void Engine::clearImGUI() {
	ImGui::End();
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}


