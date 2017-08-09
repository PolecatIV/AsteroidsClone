#version 130

attribute vec3 vertexPosition;
attribute vec2 texCoord; 


varying vec2 texCoord0;
out vec2 fragmentPosition;

uniform mat4 P;
uniform mat4 model;

void main() 
{
	gl_Position.xy = (P * model * vec4(vertexPosition, 1.0)).xy;
	gl_Position.z  = 0.0;
	gl_Position.w = 1.0;

	//fragmentPosition = vertexPosition;
	texCoord0 = texCoord;
}