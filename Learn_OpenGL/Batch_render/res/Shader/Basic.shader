#shader vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec3 normal;

out vec2 v_TexCoord;
out vec3 Normal;
out vec3 FragPos;

uniform mat4 u_MVP;

void main()
{
	gl_Position = u_MVP * vec4(position, 1.0f);
	v_TexCoord = texCoord;
	//����ռ��м���
	FragPos = vec3(u_MVP * vec4(position, 1.0f));
	Normal = normal;
};

#shader fragment
#version 330 core

struct Material
{
    sampler2D diffuse;//ambient = diffuse;��������  Խ��Խ����
    sampler2D specular;//Խ�����ǿ��Խ����
    sampler2D emission;//������ͼemission
    float shininess;
};

struct Light
{
    vec3 position;
    vec3 direction;//�÷�������ָ�����壬��������lightDirΪָ���Դ�ķ���
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float constant;//������  ˥������
    float linear;//һ����
    float quadratic;//������
    float cutoff;//�۹�Ƕ� ��Բ׶
    float outercutoff;//��Բ׶
};

out vec4 color;

in vec2 v_TexCoord;
in vec3 Normal;
in vec3 FragPos;

uniform Material material;
uniform Light light;
uniform vec3 ViewPos;

void main()
{
    vec3 lightDir = normalize(light.position - FragPos);//ָ���Դ�ķ���
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutoff - light.outercutoff;
    float intensity = clamp((theta - light.outercutoff) / epsilon, 0.0f, 1.0f);//�ѵ�һ�������̶���0.0f-1.0f֮��

    //�������� Ambient
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, v_TexCoord));
	//��������� Diffuse 
    vec3 norm = normalize(Normal);//��׼��������
    float diff = max(dot(norm, lightDir), 0.0);//ɢ������
    vec3 diffuse = intensity * diff * light.diffuse * vec3(texture(material.diffuse, v_TexCoord));
	//����߹�specular highlight
	vec3 viewDir = normalize(ViewPos - FragPos);//���߷�������
	vec3 reflectDir = reflect(-lightDir, norm);//��������  reflect����Ҫ��ĵ�һ���Ǵӹ�Դָ��Ƭ��λ�õ�����,�ڶ�������Ҫ����һ��������
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);//�߹�ķ���ֵ32��Խ��������Խǿ��ɢ��ԽС���߹��ԽС
	vec3 specular = spec * light.specular * vec3(texture(material.specular, v_TexCoord));
    specular *= intensity;
    //������ͼemission map
    //vec3 emission = vec3(texture(material.emission, v_TexCoord));
    //˥��
    float distance = length(light.position - FragPos);
    float attenuation = 1.0f / (light.constant + light.linear  *distance +light.quadratic * (distance * distance));
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
	
    color =  vec4((ambient + diffuse + specular), 1.0f);
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