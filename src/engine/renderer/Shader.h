#ifndef SHADER_H
#define SHADER_H

#include <string>

#include <glm/glm.hpp>

namespace MattEngine {

class Shader {
public:
	Shader(const std::string& path);
	void bind() const;
	void setMat4(const std::string& name, const glm::mat4& value);
	void setVec3(const std::string& name, const glm::vec3& value);
	void setVec3(const std::string& name, const unsigned int index,
		const glm::vec3& value);
	void setVec3(const std::string& name, const unsigned int index,
		const std::string& property, const glm::vec3& value);
	void setBool(const std::string& name, bool value);
	void setInt(const std::string& name, int value);
	void setFloat(const std::string& name, float value);
	void setFloat(
		const std::string& name, const unsigned int index, float value);
	void setFloat(const std::string& name, const unsigned int index,
		const std::string& property, float value);

public:
	unsigned int ShaderId;
};

} // namespace MattEngine

#endif
