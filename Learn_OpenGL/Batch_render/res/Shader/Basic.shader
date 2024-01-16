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
	//世界空间中计算
	FragPos = vec3(u_MVP * vec4(position, 1.0f));
	Normal = mat3(transpose(inverse(u_MVP))) * normal; 
};



#shader fragment
#version 330 core

struct Material
{
    sampler2D diffuse;//ambient = diffuse;环境因子  越大越鲜艳
    sampler2D specular;//越大光照强度越明亮
    //sampler2D emission;//放射贴图emission
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
    float cutoff;//聚光角度 内圆锥
    float outercutoff;//外圆锥
};

//函数
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
    vec3 norm = normalize(Normal);//标准化法向量
    vec3 viewDir = normalize(ViewPos - FragPos);//视线方向坐标
    //计算平行光照
    vec3 result = CalcDirLight(dirLight, norm, viewDir);
    //计算顶点光照
    for(int i = 0; i < NR_POINT_LIGHTS; i++)
        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);
    //计算聚光灯SpotLight
    //vec3 result = CalcSpotLight(spotLight, norm, FragPos, viewDir);

    color = vec4(result, 1.0f);
};

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // 计算漫反射强度
    float diff = max(dot(normal, lightDir), 0.0);
    // 计算镜面反射强度
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // 合并各个光照分量
    vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, v_TexCoord));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, v_TexCoord));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, v_TexCoord));
    
    return (ambient + diffuse + specular);
} 

// 计算定点光在确定位置的光照颜色
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // 计算漫反射强度
    float diff = max(dot(normal, lightDir), 0.0);
    // 计算镜面反射
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // 计算衰减
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    // 将各个分量合并
    vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, v_TexCoord));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, v_TexCoord));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, v_TexCoord));
    
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular);
}

//计算聚光灯在确定位置的光照
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);//指向光源的方向
    // 漫反射强度
    float diff = max(dot(normal, lightDir), 0.0);//散射因子
    // 镜面反射强度
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // 衰减
    float distance = length(light.position - fragPos);
    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    // 聚光灯强度
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutoff - light.outercutoff;
    float intensity = clamp((theta - light.outercutoff) / epsilon, 0.0f, 1.0f);//把第一个参数固定在0.0f-1.0f之间
    // 合并分量
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, v_TexCoord));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, v_TexCoord));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, v_TexCoord));

    ambient *= attenuation;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    
    return (ambient + diffuse + specular);
}