#include "Object.h"

Object::Object(const char* path) {
	std::vector<int>* bufferIndices = new std::vector<int>;

	m_Path = std::filesystem::path(path);
	loadFile();

	if (m_Result.attributes.positions.empty()) {
		std::cout << "3D Object empty" << std::endl;
		assert(false);
	}


	for (size_t i = 0; i < m_Result.shapes.size(); i++) {
		auto& object = m_Result.shapes.at(i);

		for (size_t j = 0; j < object.mesh.indices.size(); j++) {
			auto value = object.mesh.indices[j].position_index;
			bufferIndices->push_back(value);
		}
	}

	auto layout = VertexBufferLayout();
	layout.Push<float>(3);
	
	m_IndexBuffer.loadData(bufferIndices->data(), bufferIndices->size());
	m_VertexBuffer.loadData(m_Result.attributes.positions.data(), m_Result.attributes.positions.size());

	m_VertexArray.addData(m_VertexBuffer, layout);

	m_Position = glm::vec3(0.0f, 0.0f, 0.0f);
	m_Rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	m_ModelMatrix = glm::mat4(1.0f);
	
	delete bufferIndices;
}

Object::~Object() {

}

bool Object::loadFile() {
	m_Result = rapidobj::ParseFile(m_Path);
	bool success = rapidobj::Triangulate(m_Result);
	return success;
}

void Object::recalculateModelMatrix() {
	m_ModelMatrix = glm::translate(glm::mat4(1.0f), m_Position);
	// m_ModelMatrix = glm::rotate(m_ModelMatrix, m_Position);
}
