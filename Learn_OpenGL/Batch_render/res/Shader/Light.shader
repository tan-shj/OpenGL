#shader vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec3 normal;

out vec2 v_TexCoord;
out vec3 Normal;

uniform mat4 u_MVP;

void main()
{
	gl_Position = u_MVP * vec4(position, 1.0f);
	v_TexCoord = texCoord;
	Normal = normal;
};

#shader fragment
#version 330 core

out vec4 color;

in vec2 v_TexCoord;
in vec3 Normal;

void main()
{
	color = vec4(1.0f);
};
