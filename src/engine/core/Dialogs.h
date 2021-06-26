#ifndef DIALOGS_H
#define DIALOGS_H

#include <nfd.h>

#include <optional>
#include <string>

namespace MattEngine {

class Dialogs {
public:
	static std::optional<const std::string> open();
	static std::optional<const std::string> save();
};

} // namespace MattEngine

#endif
