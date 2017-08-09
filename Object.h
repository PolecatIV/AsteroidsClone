#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include "glm/ext.hpp"
#include <glew.h>
#include "Shader.h"
#include "OBJLoader.h"




class Vertex
{
public:
	Vertex(const glm::vec3& pos, const glm::vec2& texCoord, const glm::vec3 normal = glm::vec3(0.0f, 0.0f, 0.0f))
	{
		this->pos = pos;
		this->texCoord = texCoord;
		this->normal = normal;
	}
	inline glm::vec3* getPos() { return &pos; };
	inline glm::vec2* getTexCoord() { return &texCoord; };
	inline glm::vec3* getNormal() { return &normal; };

private:
	glm::vec3 pos;
	glm::vec2 texCoord;
	glm::vec3 normal;
};

class Mesh
{
public:
	Mesh(Vertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices);
	Mesh(const std::string& fileName);
	void draw(glm::mat4 modelMatrix, Shader* shader, GLenum drawingMode, GLfloat x, GLfloat y, GLfloat radius, bool collisionDebug);
	
	~Mesh();
private:
	void InitMesh(const IndexedModel& model);
	Mesh(const Mesh& other);
	enum
	{
		POSITION_VB,
		TEXCOORD_VB,
		NORMAL_VB,
		INDEX_VB,
		NUM_BUFFERS
	};
	GLuint _vertexArrayObject;
	GLuint _vertexArrayBuffers[NUM_BUFFERS];
	unsigned int _drawCount;
};

