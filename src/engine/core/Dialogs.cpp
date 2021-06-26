#include "Dialogs.h"

#include <optional>

using namespace MattEngine;

std::optional<const std::string> Dialogs::open() {
	nfdchar_t* path = NULL;
	nfdresult_t result = NFD_OpenDialog(NULL, NULL, &path);

	if (result == NFD_OKAY) {
		std::string pathString(path);
		return pathString;
	}

	return {};
}

std::optional<const std::string> Dialogs::save() {
	nfdchar_t* path = NULL;
	nfdresult_t result = NFD_SaveDialog(NULL, NULL, &path);

	if (result == NFD_OKAY) {
		std::string pathString(path);
		return pathString;
	}

	return {};
}