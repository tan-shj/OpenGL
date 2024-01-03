#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 color;
layout(location = 2) in vec2 texCoord;

out vec2 v_TexCoord;
out vec4 ourcolor;

void main()
{
	gl_Position = position;
	ourcolor = color;
	v_TexCoord = texCoord;
};

#shader fragment
#version 330 core

out vec4 color;

in vec2 v_TexCoord;
in vec4 ourcolor;

uniform sampler2D u_Texture1;
uniform sampler2D u_Texture2;

void main()
{
	//vec4 texColor = mix(texture(u_Texture1, v_TexCoord), texture(u_Texture2, vec2(1-v_TexCoord.x,v_TexCoord.y)), 0.2);//�޸�C�ĳ���
	vec4 texColor = mix(texture(u_Texture1, vec2(v_TexCoord.x/2,v_TexCoord.y/2)), texture(u_Texture2, v_TexCoord), 0.2);//�޸�C��С��λ��
	color = texColor * ourcolor;
};

/*��ϰ   2��3��4���
1���޸�Ƭ����ɫ��������Ц��ͼ������һ������
2�������ò�ͬ�������Ʒ�ʽ���趨һ����0.0f��2.0f��Χ�ڵģ�������ԭ����0.0f��1.0f���������ꡣ���Կ��ܲ��������ӵĽ������4��Ц�����ǵ�һ��Ҫ���������Ļ��Ʒ�ʽ��
3�������ھ�����ֻ��ʾ����ͼ����м�һ���֣��޸��������꣬�ﵽ�ܿ������������ص�Ч��������ʹ��GL_NEAREST��������˷�ʽ��������ʾ�ø�����
4��ʹ��һ��uniform������Ϊmix�����ĵ������������ı���������ɼ��ȣ�ʹ���Ϻ��¼����ı����ӻ�Ц���Ŀɼ���
*/