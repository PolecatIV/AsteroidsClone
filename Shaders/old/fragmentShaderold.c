#version 130
in vec2 fragmentPosition;
in vec2 texCoord0;

out vec4 color;

uniform float time;

varying vec2 texCoord0;

uniform sampler2D diffuse;


void main()
{
	//color = vec4(0.0, 0.0, 1.0, 1.0);	
	color = texture2D(diffuse, texCoord0);
	//color = texture2D(diffuse, vec2(0.5, 0.5));
	
}

