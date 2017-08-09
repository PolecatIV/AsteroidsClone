
#version 330 core
layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 normals;


uniform mat4 P;
uniform mat4 model;
uniform int hit;

out vec3 fragPos;
out vec2 TexCoord;
out vec3 normalsFrag;


void main()
{
	gl_Position = (P * model * vec4(vertexPosition, 1.0));

	TexCoord = vec2(texCoord.x, 1.0 - texCoord.y);
	normalsFrag = mat3(transpose(inverse(model))) * normals;  
	fragPos = vec3(model * vec4(vertexPosition, 1.0f));
}
