#ifndef CUBE_MAP_H
#define CUBE_MAP_H

#include <string>

namespace MattEngine {

class CubeMap {
public:
	CubeMap(const std::string& directory);
	void bind() const;
	void unbind() const;

private:
	unsigned int m_textureId;
};

} // namespace MattEngine

#endif
