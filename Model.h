#pragma once
#include <glew.h>
#include "Entity.h"

class Model
{
public:
	Model(GLuint vaoID, GLuint vboID, int vertexCount);
	Model();
	~Model();
	GLuint getVaoID();
	GLuint getVboID();
	glm::mat4 getObjectMatrix();
	void setObjectMatrix(glm::mat4 matrix);
	int getVertexCount();
private:
	GLuint _vaoID;
	GLuint _vboID;
	glm::mat4 _objectMatrix;
	int _vertexCount;
};

