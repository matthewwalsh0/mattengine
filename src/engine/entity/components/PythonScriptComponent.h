#ifndef PYTHON_SCRIPT_COMPONENT_H
#define PYTHON_SCRIPT_COMPONENT_H

#include "PythonScript.h"

#include <optional>

using namespace MattEngine;

struct PythonScriptComponent {
	std::string Path = "";
	std::optional<PythonScript> Script;

	PythonScriptComponent() {}
	PythonScriptComponent(const std::string& path) : Path(path) {}
};

#endif
