#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;

out vec2 v_TexCoord;

uniform vec4 translation;

void main()
{
	//gl_Position = vec4(position.x, -position.y, position.z, position.w);//修改顶点着色器让三角形上下颠倒
	gl_Position = position + translation;//使用uniform定义一个水平偏移量，在顶点着色器中使用这个偏移量把三角形移动到屏幕右侧
	//是用out把定点位置输出到片段着色器，并把片段的颜色设置为与顶点位置相等。

	//gl_Position = position;
	v_TexCoord = texCoord;
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;

uniform vec4 u_Color;
uniform sampler2D u_Texture;

void main()
{
	vec4 texColor = texture(u_Texture, v_TexCoord);
	color = texColor;
};