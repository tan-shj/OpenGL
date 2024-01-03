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
	//vec4 texColor = mix(texture(u_Texture1, v_TexCoord), texture(u_Texture2, vec2(1-v_TexCoord.x,v_TexCoord.y)), 0.2);//修改C的朝向
	vec4 texColor = mix(texture(u_Texture1, vec2(v_TexCoord.x/2,v_TexCoord.y/2)), texture(u_Texture2, v_TexCoord), 0.2);//修改C大小，位置
	color = texColor * ourcolor;
};

/*练习   2、3、4差不多
1、修改片段着色器，仅让笑脸图案朝另一个方向看
2、尝试用不同的纹理环绕方式，设定一个从0.0f到2.0f范围内的（而不是原来的0.0f到1.0f）纹理坐标。试试看能不能在箱子的角落放置4个笑脸。记得一定要试试其他的环绕方式。
3、尝试在矩形上只显示纹理图像的中间一部分，修改纹理坐标，达到能看见单个的像素的效果。尝试使用GL_NEAREST的纹理过滤方式让像素显示得更清晰
4、使用一个uniform变量作为mix函数的第三个参数来改变两个纹理可见度，使用上和下键来改变箱子或笑脸的可见度
*/