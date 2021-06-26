#ifndef TAG_COMPONENT_H
#define TAG_COMPONENT_H

#include <string>

struct TagComponent {
	std::string Tag = "New Entity";

	TagComponent() {}
	TagComponent(const std::string& tag) : Tag(tag) {}
	TagComponent(const TagComponent& other) : Tag(other.Tag) {}
};

#endif
