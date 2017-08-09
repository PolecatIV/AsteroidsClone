#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glew.h>

class Camera
{
public:
	Camera(const glm::vec3& pos, GLfloat cameraFOV, GLfloat cameraAspect, GLfloat camerazNear, GLfloat camerazFar)
	{
		fov = cameraFOV;
		aspect = cameraAspect;
		zNear = camerazNear;
		zFar = camerazFar;

		_perspective = glm::perspective(fov, aspect, zNear, zFar);
		_ortho = glm::ortho(0.0f, 1280.0f, 0.0f, 720.0f);
		position = pos;
		_foward = glm::vec3(0.0f, 0.0f, -1.0f);
		_up = glm::vec3(0.0f, 1.0f, 0.0f);

		orthoProjection = _ortho;
		viewProjection = _ortho * glm::lookAt(position, position + _foward, _up);
	}

	glm::vec3 position;

	void Update()
	{
		_perspective = glm::perspective(fov, aspect, zNear, zFar);
		viewProjection = _perspective * glm::lookAt(position, position + _foward, _up);
	}
	
	inline glm::mat4 GetViewProjection() const
	{
		return _perspective * glm::lookAt(_position, _position + _foward, _up);
	}
	GLfloat fov, aspect, zNear, zFar;
	glm::mat4 viewProjection;
	glm::mat4 orthoProjection;
private:
	glm::mat4 _ortho;
	glm::mat4 _perspective;
	glm::vec3 _position;
	glm::vec3 _foward;
	glm::vec3 _up;
};

