#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;

out vec2 v_TexCoord;

uniform mat4 u_MVP;

void main()
{
	gl_Position = u_MVP * position;
	v_TexCoord = texCoord;
};

#shader fragment
#version 330 core

out vec4 color;

in vec2 v_TexCoord;

uniform sampler2D u_Texture1;
uniform sampler2D u_Texture2;
uniform vec3 ObjectColor;
uniform vec3 LightColor;

void main()
{
	vec4 texColor = mix(texture(u_Texture1, v_TexCoord), texture(u_Texture2, v_TexCoord), 0.2);
	vec4 ourColor = vec4(LightColor * ObjectColor, 1.0f);
	color = texColor * ourColor;
};
