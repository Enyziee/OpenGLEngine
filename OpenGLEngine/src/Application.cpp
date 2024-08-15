#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Log.h"
#include "Camera.h"
#include "Renderer.h"
#include <rapidobj/rapidobj.hpp>
#include "ShaderProgram.h"
#include "Object.h"


static void APIENTRY glDebugOutput(GLenum source,
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
}

bool firstMouse = true;
bool mouseEnabled = false;

static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	switch (key) {
	case GLFW_KEY_ESCAPE:
		glfwSetWindowShouldClose(window, true);
		break;
	case GLFW_KEY_1:
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		mouseEnabled = true;
		break;
	case GLFW_KEY_2:
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		mouseEnabled = false;
		firstMouse = true;
		break;

	default:
		break;
	}

}


float deltaTime = 0.0f;
float lastFrame = 0.0f;

static void processInput(GLFWwindow* window, Camera& camera, float deltaTime) {
	const float cameraSpeed = 4.0f * deltaTime;

	glm::vec3 cameraPosition = camera.getPosition();
	glm::vec3 cameraDirection = camera.getDirection();
	glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		cameraPosition += cameraSpeed * camera.getDirection();
	}
		
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		cameraPosition -= cameraSpeed * camera.getDirection();
	}

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		cameraPosition -= glm::normalize(glm::cross(cameraDirection, worldUp)) * cameraSpeed;
	}

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		cameraPosition += glm::normalize(glm::cross(cameraDirection, worldUp)) * cameraSpeed;
	}

	camera.setPosition(cameraPosition);
}



float lastX = 640, lastY = 360;

static float cameraRotation[3] = { 0.0f, 0.0f, -1.0f };

static void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
	if (!mouseEnabled) {
		return;
	}

	float xpos = (float)(xposIn);
	float ypos = (float)(yposIn);

	if (firstMouse) {
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.1f; // change this value to your liking
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	float yaw = cameraRotation[0];
	float pitch = cameraRotation[1];

	yaw += xoffset; // yaw
	pitch += yoffset;

	// make sure that when pitch is out of bounds, screen doesn't get flipped
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	cameraRotation[0] = yaw;
	cameraRotation[1] = pitch;
}


int main() {
	GLFWwindow* m_Window = nullptr;
	unsigned int m_Width = 1280;
	unsigned int m_Height = 720;

	Logger::Init();
	
	if (!glfwInit()) {
		LOGGER_ERROR("Failed to initialize GLFW");
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

	m_Window = glfwCreateWindow(m_Width, m_Height, "Hello World", NULL, NULL);
	if (!m_Window) {
		LOGGER_ERROR("Failed to create GLFW Window");
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(m_Window);
	glfwSetKeyCallback(m_Window, keyCallback);
	glfwSetCursorPosCallback(m_Window, mouse_callback);

	

	glfwSwapInterval(1);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

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

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad 
	ImGui::StyleColorsDark();

	ImGui_ImplGlfw_InitForOpenGL(m_Window, true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
	ImGui_ImplOpenGL3_Init();

	// GL Stuff

	//auto cube = Object("./res/objects/cube.obj");
	//auto teapot = rapidobj::ParseFile("./res/objects/cube.obj");
	//LOGGER_TRACE(teapot.error.code.message());
	//std::vector<unsigned int>* bufferIndices = new std::vector<unsigned int>;

	//for (size_t i = 0; i < teapot.shapes.size(); i++) {
	//	auto& object = teapot.shapes.at(i);

	//	for (size_t j = 0; j < object.mesh.indices.size(); j++) {
	//		auto value = object.mesh.indices[j].position_index;
	//		bufferIndices->push_back(value);
	//	}
	//}


	float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};


	ShaderProgram shader = ShaderProgram("./res/shader/default.vert", "./res/shader/default.frag");


	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texture coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glm::vec3 v = glm::normalize(glm::vec3(0.0f, 0.0f, -3.0f) - glm::vec3(1.0f, 2.0f, 0.0f));

	LOGGER_INFO("Look vector: {0}, {1}, {2}", v.x, v.y, v.z);
	
	// Engine variables

	Renderer renderer = Renderer();
	Camera camera = Camera(1280.0f, 720.0f, 0.1f, 100.0f);
	camera.setPosition({ 0.0f, 0.0f, 3.0f });
	camera.setRotation({ -90.0f, 0.0f, -0.0f });



	while (!glfwWindowShouldClose(m_Window)) {
		
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(m_Window, camera, deltaTime);

		// GL CODE 
		renderer.clear();
		

		camera.setRotation({ cameraRotation[0], cameraRotation[1], cameraRotation[2] });


		shader.bind();
		shader.setUniformMatrix4fv("u_ViewProjection", camera.getViewProjectionMatrix());

		
		glBindVertexArray(VAO);
		for (unsigned int i = 0; i < 10; i++) {
			// calculate the model matrix for each object and pass it to shader before drawing
			glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			shader.setUniformMatrix4fv("u_Model", model);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}


		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// ImGUI Interface 
		

		ImGui::Begin("Info");
		ImGui::Text("Camera Position (%.2f %.2f %.2f)", camera.getPosition().x, camera.getPosition().y, camera.getPosition().z);
		ImGui::Text("Camera Direction (%.2f %.2f %.2f)", camera.getDirection().x, camera.getDirection().y, camera.getDirection().z);
		ImGui::Text("Camera Euler (%.2f %.2f %.2f)", camera.getRotation().x, camera.getRotation().y, camera.getRotation().z);

		ImGui::SliderFloat3(" Camera Rotation", cameraRotation, -90.0f, 90.0f);

		ImGui::End();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(m_Window);
		glfwPollEvents();
	}
}