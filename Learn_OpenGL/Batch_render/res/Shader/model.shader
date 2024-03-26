#shader vertex
#version 330 core

layout(location = 0)in vec3 Position;
layout(location = 1)in vec3 Normal;
layout(location = 2)in vec2 TexCoord;


out vec2 v_TextCoord;

uniform mat4 u_MVP;

void main()
{
	gl_Position = u_MVP * vec4(Position, 1.0f);
	v_TextCoord = TexCoord;
};


//////////////////////////////////////////////////////////////////

#shader fragment
#version 330 core

in vec2 v_TextCoord;
out vec4 color;

uniform sampler2D texture_diffuse1;

void main()
{
	color = vec4(texture(texture_diffuse1, v_TextCoord));
};
