#pragma once
#include "glad/glad.h"
#include <string>
#include <glm.hpp>


class Shader
{
public:
	
	Shader();
	~Shader();

	void CreateShaderProgram(const char* vertexPath, const char* fragmentPath);
	void DeleteShaderProgram();

	void setVertexAttribPointer(const char* param, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer) const;

	void Use() const;

	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
	void set3Float(const std::string& name, float v0, float v1, float v2) const;
	void set4Float(const std::string& name, float v0, float v1, float v2, float v4) const;
	void setMat4(const std::string& name, const glm::mat4& mat) const;

	GLuint GetId() const { return id; }

private:
	GLuint compileShader(GLenum type, const char* source);
	void checkCompileErrors(GLuint shader, const std::string& type) const;

private:
	GLuint id;
};
