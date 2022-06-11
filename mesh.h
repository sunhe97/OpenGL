#pragma once
#ifndef __Mesh_H__
#define __Mesh_H__
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include <string>
#include <vector>

using namespace std;
struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};//��Ҫ���뵽������ɫ���Ĳ���

struct Texture 
{
	unsigned int id;
	std::string type;
	aiString path;
};
class Mesh {
public:
	vector<Vertex> vertices;
	vector<Texture> textures;
	vector<unsigned int> indices;
	Mesh(vector<Vertex>& vertices,vector<Texture>& texture,vector<unsigned int>& indices) {
		this->vertices = vertices;
		this->textures = texture;
		this->indices = indices;
		setupMesh();
	}
	/// <summary>
	/// ����������
	/// </summary>
	/// <param name="shader"></param>
	void Draw(Shader shader) {
		unsigned int diffuseNr = 1;
		unsigned int specularNr = 1;
		for (int i = 0; i < textures.size(); i++) {
			glActiveTexture(GL_TEXTURE0 + i);
			string number;
			string name = textures[i].type;
			if (name == "texture_diffuse")
				number = to_string(diffuseNr++);
			else if (name == "texture_specular")
			{
				number = to_string(specularNr++);
			}
			shader.setFloat("material." + name + number, i);//ΪʲôҪ��c_str()
			glBindTexture(GL_TEXTURE_2D, textures[i].id);
			
		}
		

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);//GLDRAWELEMENTS ���һ��Ԫ�صı�ʾ
		glBindVertexArray(0);
		glActiveTexture(GL_TEXTURE0);
	}
private:
	unsigned int VAO, VBO, EBO;
	/// <summary>
	/// ���ڽ����ݰ�
	/// </summary>
	void setupMesh() {
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

		glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

		//��������
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(0));
		glEnableVertexAttribArray(0);
		//���㷨��
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
		glEnableVertexAttribArray(1);
		//������������
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
		glEnableVertexAttribArray(2);

		glBindVertexArray(0);
	}
};
#endif // !__Mesh_H__
