#include "Model.h"

Model::Model()
{
	this->_vaoID = 0;
	this->_vboID = 0;
	this->_vertexCount = 0;

}
Model::Model(GLuint vaoID, GLuint vboID, int vertexCount)
{
	this->_vaoID = vaoID;
	this->_vboID = vboID;
	this->_vertexCount = vertexCount;

}

glm::mat4 Model::getObjectMatrix()
{
	return _objectMatrix;
}

void Model::setObjectMatrix(glm::mat4 matrix)
{
	_objectMatrix = matrix;
}

GLuint Model::getVaoID()
{
	return _vaoID;
}

GLuint Model::getVboID()
{
	return _vboID;
}

int Model::getVertexCount()
{
	return _vertexCount;
}


Model::~Model()
{
}
