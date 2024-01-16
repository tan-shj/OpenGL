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
	Normal = mat3(transpose(inverse(u_MVP))) * normal; 
};



#shader fragment
#version 330 core

struct Material
{
    sampler2D diffuse;//ambient = diffuse;��������  Խ��Խ����
    sampler2D specular;//Խ�����ǿ��Խ����
    //sampler2D emission;//������ͼemission
    float shininess;
};

struct DirLight 
{
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
}; 

struct PointLight 
{
    vec3 position;

    float constant;
    float linear;
    float quadratic;  

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  

struct SpotLight
{
    vec3 position;
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    
    float constant;
    float linear;
    float quadratic;
    float cutoff;//�۹�Ƕ� ��Բ׶
    float outercutoff;//��Բ׶
};

//����
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

out vec4 color;

in vec2 v_TexCoord;
in vec3 Normal;
in vec3 FragPos;

#define NR_POINT_LIGHTS 1
uniform Material material;
uniform DirLight dirLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight spotLight;
uniform vec3 ViewPos;

void main()
{
    vec3 norm = normalize(Normal);//��׼��������
    vec3 viewDir = normalize(ViewPos - FragPos);//���߷�������
    //����ƽ�й���
    vec3 result = CalcDirLight(dirLight, norm, viewDir);
    //���㶥�����
    for(int i = 0; i < NR_POINT_LIGHTS; i++)
        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);
    //����۹��SpotLight
    //vec3 result = CalcSpotLight(spotLight, norm, FragPos, viewDir);

    color = vec4(result, 1.0f);
};

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // ����������ǿ��
    float diff = max(dot(normal, lightDir), 0.0);
    // ���㾵�淴��ǿ��
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // �ϲ��������շ���
    vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, v_TexCoord));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, v_TexCoord));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, v_TexCoord));
    
    return (ambient + diffuse + specular);
} 

// ���㶨�����ȷ��λ�õĹ�����ɫ
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // ����������ǿ��
    float diff = max(dot(normal, lightDir), 0.0);
    // ���㾵�淴��
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // ����˥��
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    // �����������ϲ�
    vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, v_TexCoord));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, v_TexCoord));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, v_TexCoord));
    
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular);
}

//����۹����ȷ��λ�õĹ���
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);//ָ���Դ�ķ���
    // ������ǿ��
    float diff = max(dot(normal, lightDir), 0.0);//ɢ������
    // ���淴��ǿ��
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // ˥��
    float distance = length(light.position - fragPos);
    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    // �۹��ǿ��
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutoff - light.outercutoff;
    float intensity = clamp((theta - light.outercutoff) / epsilon, 0.0f, 1.0f);//�ѵ�һ�������̶���0.0f-1.0f֮��
    // �ϲ�����
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, v_TexCoord));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, v_TexCoord));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, v_TexCoord));

    ambient *= attenuation;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    
    return (ambient + diffuse + specular);
}