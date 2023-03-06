#pragma once

constexpr float vertices[] = {
	-0.5f, 0.5f, 0.0f, 0.0f, 0.0f, // top left
	0.5f, 0.5f, 0.0f, 1.0f, 0.0f, // top right
	0.5f, -0.5f, 0.0f, 1.0f, 1.0f, // bottom right
	-0.5f, -0.5f, 0.0f, 0.0f, 1.0f // bottom left
};

constexpr float spriteVertices[] = {
	-0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, // Top Left
	0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, // Top Right
	0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, // Bottom Right
	-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f // Bottom Left
};

constexpr unsigned int indices[] = {
	0, 1, 2,
	2, 3, 0
};

class VertexArray
{
public:
	VertexArray(const float* verticesP, unsigned int nbVerticesP,
		const unsigned int* indicesP, unsigned int nbIndicesP);
	~VertexArray();

	void setActive();

	unsigned int getNbVertices() const { return nbVertices; }
	unsigned int getNbIndices() const { return nbIndices; }

private:
	unsigned int nbVertices;
	unsigned int nbIndices;

	// OpenGL ID of the vertex array object
	unsigned int vertexArray;
	// OpenGL ID of the vertex buffer object
	unsigned int vertexBuffer;
	// OpenGL ID of the index buffer object
	unsigned int indexBuffer;
};