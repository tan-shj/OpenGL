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
	Normal = normal;
};

#shader fragment
#version 330 core

struct Material
{
    sampler2D diffuse;//ambient = diffuse;环境因子  越大越鲜艳
    sampler2D specular;//越大光照强度越明亮
    sampler2D emission;//放射贴图emission
    float shininess;
};

struct Light
{
    vec3 position;
    vec3 direction;//该方向向量指向物体，下面计算的lightDir为指向光源的方向
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float constant;//常数项  衰减方程
    float linear;//一次项
    float quadratic;//二次项
    float cutoff;//聚光角度 内圆锥
    float outercutoff;//外圆锥
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
    vec3 lightDir = normalize(light.position - FragPos);//指向光源的方向
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutoff - light.outercutoff;
    float intensity = clamp((theta - light.outercutoff) / epsilon, 0.0f, 1.0f);//把第一个参数固定在0.0f-1.0f之间

    //环境光照 Ambient
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, v_TexCoord));
	//漫反射光照 Diffuse 
    vec3 norm = normalize(Normal);//标准化法向量
    float diff = max(dot(norm, lightDir), 0.0);//散射因子
    vec3 diffuse = intensity * diff * light.diffuse * vec3(texture(material.diffuse, v_TexCoord));
	//镜面高光specular highlight
	vec3 viewDir = normalize(ViewPos - FragPos);//视线方向坐标
	vec3 reflectDir = reflect(-lightDir, norm);//反射坐标  reflect函数要求的第一个是从光源指向片段位置的向量,第二个参数要求是一个法向量
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);//高光的发光值32，越大反射能力越强，散射越小，高光点越小
	vec3 specular = spec * light.specular * vec3(texture(material.specular, v_TexCoord));
    specular *= intensity;
    //放射贴图emission map
    //vec3 emission = vec3(texture(material.emission, v_TexCoord));
    //衰减
    float distance = length(light.position - FragPos);
    float attenuation = 1.0f / (light.constant + light.linear  *distance +light.quadratic * (distance * distance));
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
	
    color =  vec4((ambient + diffuse + specular), 1.0f);
};

//Gouraud光照替换冯氏光照
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