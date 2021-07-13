#include "Shader.h"

#include <fstream>
#include <sstream>
#include <string>

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include "Log.h"

using namespace MattEngine;

namespace Utils {

static std::string readFile(const std::string& path) {
	std::ifstream file(path);
	std::stringstream buffer;
	buffer << file.rdbuf();
	return buffer.str();
}

static std::string readVertexSource(const std::string& data) {
	return data.substr(0, data.find("#separator"));
}

static std::string readFragmentSource(const std::string& data) {
	return data.substr(data.find("#separator") + 10);
}

static GLuint compileSingle(const std::string& source, int shaderType) {
	GLuint shaderId = glCreateShader(shaderType);
	const GLchar* sourceOld = source.c_str();

	glShaderSource(shaderId, 1, &sourceOld, 0);
	glCompileShader(shaderId);

	GLint isCompiled = 0;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &isCompiled);

	if (isCompiled == GL_FALSE) {
		int logLength = 0;
		glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &logLength);

		char* error = new char[logLength];

		glGetShaderInfoLog(shaderId, logLength, NULL, error);

		MATTENGINE_ASSERT(false, "Could not compile shader. | %s", error);
	}

	return shaderId;
}

static GLuint compile(const std::string& source) {
	GLuint vertexShaderId =
		compileSingle(readVertexSource(source), GL_VERTEX_SHADER);

	GLuint fragmentShaderId =
		compileSingle(readFragmentSource(source), GL_FRAGMENT_SHADER);

	GLuint shaderId = glCreateProgram();
	glAttachShader(shaderId, vertexShaderId);
	glAttachShader(shaderId, fragmentShaderId);
	glLinkProgram(shaderId);

	GLint isLinked = 0;
	glGetProgramiv(shaderId, GL_LINK_STATUS, (int*)&isLinked);
	MATTENGINE_ASSERT(isLinked == GL_TRUE, "Could not link shader.", NULL);

	glDetachShader(shaderId, vertexShaderId);
	glDetachShader(shaderId, fragmentShaderId);

	return shaderId;
}

} // namespace Utils

Shader::Shader(const std::string& path) {
	std::string data = Utils::readFile(path);
	ShaderId = Utils::compile(data);
}

void Shader::bind() const { glUseProgram(ShaderId); }

void Shader::setMat4(const std::string& name, const glm::mat4& value) {
	glUniformMatrix4fv(glGetUniformLocation(ShaderId, name.c_str()), 1,
		GL_FALSE, glm::value_ptr(value));
}

void Shader::setVec3(const std::string& name, const glm::vec3& value) {
	glUniform3f(glGetUniformLocation(ShaderId, name.c_str()), value.x, value.y,
		value.z);
}

void Shader::setBool(const std::string& name, bool value) {
	glUniform1i(glGetUniformLocation(ShaderId, name.c_str()), value);
}

void Shader::setInt(const std::string& name, int value) {
	glUniform1i(glGetUniformLocation(ShaderId, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) {
	glUniform1f(glGetUniformLocation(ShaderId, name.c_str()), value);
}