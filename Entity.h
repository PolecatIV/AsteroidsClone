#pragma once
#include <glew.h>
#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include "glm/ext.hpp"
#include "Object.h"
#include "Texture.h"

struct Ship
{
	GLfloat width = 0.0f;
	GLfloat height = 0.0f;
	GLfloat x = 0.0f;
	GLfloat y = 0.0f;

	glm::vec3 pos;
	glm::vec3 vel = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 direction = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 inerthia = glm::vec3(0.0f, 0.0f, 0.0f);
	GLfloat thrust = 0.0f;
	GLfloat acceleration = 0.1f;
	GLfloat speed = 0.0f;
	GLfloat angle = 0.0f;
	GLfloat angleVel = 4.0f;
	GLfloat radius = 0.0f;
	GLfloat scaleFactor = 1.0f;

	int hit = 0;
	int energy = 0;
	bool isAlive = true;
	bool accelerating = false;
	bool decelerating = false;
	bool turningLeft = false;
	bool turningRight = false;
	bool fire = false;

	Texture* texture;
	Mesh* mesh;
};
