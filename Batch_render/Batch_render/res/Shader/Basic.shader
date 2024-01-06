#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
//layout(location = 1) in vec4 color;
layout(location = 1) in vec2 texCoord;

out vec2 v_TexCoord;
//out vec4 ourcolor;

uniform mat4 u_MVP;

void main()
{
	gl_Position = u_MVP * position;
//	ourcolor = color;
	v_TexCoord = texCoord;
};

#shader fragment
#version 330 core

out vec4 color;

in vec2 v_TexCoord;
//in vec4 ourcolor;

uniform sampler2D u_Texture1;
uniform sampler2D u_Texture2;

void main()
{
	vec4 texColor = mix(texture(u_Texture1, v_TexCoord), texture(u_Texture2, v_TexCoord), 0.2);
	//color = texColor * ourcolor;
	color = texColor;
};
