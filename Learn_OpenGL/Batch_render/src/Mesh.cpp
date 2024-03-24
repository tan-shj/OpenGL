#include <gl/glew.h>
#include <sstream>
#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture> textures)
    :vertices(vertices), indices(indices), textures(textures)
{
    setupMesh();
}

void Mesh::setupMesh()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex),
        &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint),
        &indices[0], GL_STATIC_DRAW);

    // ���ö�������ָ��
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
        (GLvoid*)0);
    // ���÷���ָ��
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
        (GLvoid*)offsetof(Vertex, Normal));
    // ���ö������������
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
        (GLvoid*)offsetof(Vertex, TexCoords));

    glBindVertexArray(0);
}

void Mesh::Draw(Shader shader)
{
    GLuint diffuseNr = 1;
    GLuint specularNr = 1;
    for (GLuint i = 0; i < textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i); // �ڰ�����ǰ��Ҫ�����ʵ�������Ԫ
        // �����������к� (N in diffuse_textureN)
        std::stringstream ss;
        std::string number;
        std::string name = textures[i].type;
        if (name == "texture_diffuse")
            ss << diffuseNr++; // ��GLuin���뵽string stream
        else if (name == "texture_specular")
            ss << specularNr++; // ��GLuin���뵽string stream
        number = ss.str();
        shader.SetUniform1i(("material." + name + number).c_str(), i);
        glBindTexture(GL_TEXTURE_2D, textures[i].id);
    }
    glActiveTexture(GL_TEXTURE0);

    // ����Mesh
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    for (GLuint i = 0; i < textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}