#shader vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;
layout(location = 1) in vec3 normal;

out vec2 v_TexCoord;
out vec3 Normal;
out vec3 FragPos;

//观察空间用到的参数
//uniform vec3 ViewPos;
//uniform vec3 lightPos;
//out vec3 LightPos;

uniform mat4 u_MVP;

void main()
{
	gl_Position = u_MVP * vec4(position, 1.0f);
	v_TexCoord = texCoord;
	//世界空间中计算
	FragPos = vec3(u_MVP * vec4(position, 1.0f));
	Normal = normal;
	
	//在观察空间中计算
	//FragPos = vec3(ViewPos * u_MVP * vec4(position, 1.0f));
	//Normal = mat3(transpose(inverse(u_MVP))) * normal;//正规矩阵--解决法向量错误缩放效果--inverse操作开销大，建议在CPU上计算出来在传给GPU
	//LightPos = vec3(ViewPos * vec4(lightPos, 1.0f));
};

#shader fragment
#version 330 core

out vec4 color;

in vec2 v_TexCoord;
in vec3 Normal;
in vec3 FragPos;

uniform sampler2D u_Texture1;
uniform sampler2D u_Texture2;
uniform vec3 ObjectColor;
uniform vec3 LightColor;
uniform vec3 LightPos;
uniform vec3 ViewPos;

//观察空间
//in vec3 LightPos

void main()
{
	//Texture
	vec4 texColor = mix(texture(u_Texture1, v_TexCoord), texture(u_Texture2, v_TexCoord), 0.2);
	//环境光照 Ambient
	float AmbientStrength = 0.1f;//环境因子  越大越鲜艳
	vec3 ambient = AmbientStrength * LightColor;
	//漫反射光照 Diffuse 
    vec3 norm = normalize(Normal);//标准化法向量
    vec3 lightDir = normalize(LightPos - FragPos);//光的方向
    float diff = max(dot(norm, lightDir), 0.0);//散射因子
    vec3 diffuse = diff * LightColor;
	//镜面高光specular highlight
	float SpecularStrength = 0.5f;//越大光照强度越明亮
	vec3 viewDir = normalize(ViewPos - FragPos);//视线方向坐标
	//vec3 viewDir = normalize(- FragPos);//视线方向坐标
	vec3 reflectDir = reflect(-lightDir, norm);//反射坐标  reflect函数要求的第一个是从光源指向片段位置的向量,第二个参数要求是一个法向量
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);//高光的发光值32，越大反射能力越强，散射越小，高光点越小
	vec3 specular = SpecularStrength * spec * LightColor;

	color = texColor * vec4((ambient + diffuse + specular) * ObjectColor, 1.0f);
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