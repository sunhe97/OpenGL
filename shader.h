#pragma once
#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader {
public:
	unsigned int ID;
	Shader(const char* vertexShaderPath,const char* fragmentShaderPath);
	~Shader();
	void use() { glUseProgram(ID); }
	void setBool(const std::string& name, bool value) const { glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value); }
	void setInt(const std::string& name, int value) const { glUniform1i(glGetUniformLocation(ID, name.c_str()), value); }
	void setFloat(const std::string& name, float value) const { glUniform1f(glGetUniformLocation(ID, name.c_str()), value); }
	void setMat4(const std::string& name, glm::mat4 matrix) const { glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()),1,GL_FALSE,glm::value_ptr(matrix)); }
	void setvec3(const std::string& name, const glm::vec3 vec) const { glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(vec)); }

private:
	void checkCompileError(unsigned int shader, std::string type) {
		int success;
		char infoLog[1024];
		if (type != "PROGRAM") {
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (!success) {
				std::cout << "ERROR:Shader Compile Failed type:" << type<<std::endl<<infoLog<<std::endl;
				glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			}
		}
		else {
			glGetProgramiv(shader, GL_LINK_STATUS, &success);
			if (!success) {
				std::cout << "ERROR:Program Link Failed type:" << type << std::endl << infoLog << std::endl;
				glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			}
		}
	}
};


#endif // !SHADER_H
