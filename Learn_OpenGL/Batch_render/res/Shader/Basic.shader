#shader vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec3 normal;

out vec2 v_TexCoord;
out vec3 Normal;
out vec3 FragPos;

//�۲�ռ��õ��Ĳ���
//uniform vec3 ViewPos;
//uniform vec3 lightPos;
//out vec3 LightPos;

uniform mat4 u_MVP;

void main()
{
	gl_Position = u_MVP * vec4(position, 1.0f);
	v_TexCoord = texCoord;
	//����ռ��м���
	FragPos = vec3(u_MVP * vec4(position, 1.0f));
	Normal = normal;
	
	//�ڹ۲�ռ��м���
	//FragPos = vec3(ViewPos * u_MVP * vec4(position, 1.0f));
	//Normal = mat3(transpose(inverse(u_MVP))) * normal;//�������--�����������������Ч��--inverse���������󣬽�����CPU�ϼ�������ڴ���GPU
	//LightPos = vec3(ViewPos * vec4(lightPos, 1.0f));
};

#shader fragment
#version 330 core

struct Material
{
    vec3 ambient;//��������  Խ��Խ����
    vec3 diffuse;
    vec3 specular;//Խ�����ǿ��Խ����
    float shininess;
};
uniform Material material;

struct Light
{
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform Light light;

out vec4 color;

in vec2 v_TexCoord;
in vec3 Normal;
in vec3 FragPos;

uniform sampler2D u_Texture1;
uniform sampler2D u_Texture2;
uniform vec3 ViewPos;

//�۲�ռ�
//in vec3 LightPos

void main()
{
	//Texture
	vec4 texColor = mix(texture(u_Texture1, v_TexCoord), texture(u_Texture2, v_TexCoord), 0.2);
	//�������� Ambient
	vec3 ambient = material.ambient * light.ambient;
	//��������� Diffuse 
    vec3 norm = normalize(Normal);//��׼��������
    vec3 lightDir = normalize(light.position - FragPos);//��ķ���
    float diff = max(dot(norm, lightDir), 0.0);//ɢ������
    vec3 diffuse = material.diffuse * diff * light.diffuse;
	//����߹�specular highlight
	vec3 viewDir = normalize(ViewPos - FragPos);//���߷�������
	//vec3 viewDir = normalize(- FragPos);//���߷�������
	vec3 reflectDir = reflect(-lightDir, norm);//��������  reflect����Ҫ��ĵ�һ���Ǵӹ�Դָ��Ƭ��λ�õ�����,�ڶ�������Ҫ����һ��������
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);//�߹�ķ���ֵ32��Խ��������Խǿ��ɢ��ԽС���߹��ԽС
	vec3 specular = material.specular * spec * light.specular;

	color = texColor * vec4((ambient + diffuse + specular), 1.0f);
};

//Gouraud�����滻���Ϲ���
/*
// Vertex shader:
// ================
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 LightingColor; // resulting color from lighting calculations

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    
    // gouraud shading
    // ------------------------
    vec3 Position = vec3(model * vec4(aPos, 1.0));
    vec3 Normal = mat3(transpose(inverse(model))) * aNormal;
    
    // ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - Position);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    
    // specular
    float specularStrength = 1.0; // this is set higher to better show the effect of Gouraud shading 
    vec3 viewDir = normalize(viewPos - Position);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;      

    LightingColor = ambient + diffuse + specular;
}


// Fragment shader:
// ================
#version 330 core
out vec4 FragColor;

in vec3 LightingColor; 

uniform vec3 objectColor;

void main()
{
   FragColor = vec4(LightingColor * objectColor, 1.0);
}

*/