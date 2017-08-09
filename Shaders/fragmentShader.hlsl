#version 330 core
in vec2 TexCoord;
in vec3 normalsFrag;
in vec3 FragPos;

out vec4 color;

uniform int hit;
uniform sampler2D ourTexture;
uniform float xLight;

void main()
{
	vec3 ambientColor = vec3(0.5, 0.5, 0.5);

	vec3 lightPos2 = vec3(3000,0,-10);
	vec3 lightCol2 = vec3(1.0,1.0,0.0);

	vec3 lightPos3 = vec3(0,0,10);
	vec3 lightCol3 = vec3(0.6,0.6,0.6);
	

	vec3 lightPos = vec3(xLight,0,-10);
	vec3 lightCol = vec3(0.0,0.0,1.0);

	vec4 objectColor = texture(ourTexture, TexCoord);

	vec3 norm = normalize(normalsFrag);
    vec3 lightDir = normalize(lightPos - FragPos);
    vec3 lightDir2 = normalize(lightPos2 - FragPos);
    vec3 lightDir3 = normalize(lightPos3 - FragPos);

    float diff = max(dot(norm, lightDir), 0.0);
    float diff2 = max(dot(norm, lightDir2), 0.0);
    float diff3 = max(dot(norm, lightDir3), 0.0);

    vec3 diffuse = (diff * lightCol);
    vec3 diffuse2 = (diff2 * lightCol2);
    vec3 diffuse3 = (diff3 * lightCol3);

	vec3 ambient = 0.5f * ambientColor;
	 
	vec4 final =  vec4((ambient + diffuse + diffuse2 + diffuse3), 1.0) * objectColor ; 
	if(hit == 1)
	{
		final = final * vec4(0.8, 0.0, 0.0, 1.0);
	}
	color = final;
}
