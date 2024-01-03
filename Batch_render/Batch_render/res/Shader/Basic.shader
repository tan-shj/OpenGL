#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;

out vec2 v_TexCoord;

uniform vec4 translation;

void main()
{
	//gl_Position = vec4(position.x, -position.y, position.z, position.w);//�޸Ķ�����ɫ�������������µߵ�
	gl_Position = position + translation;//ʹ��uniform����һ��ˮƽƫ�������ڶ�����ɫ����ʹ�����ƫ�������������ƶ�����Ļ�Ҳ�
	//����out�Ѷ���λ�������Ƭ����ɫ��������Ƭ�ε���ɫ����Ϊ�붥��λ����ȡ�

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