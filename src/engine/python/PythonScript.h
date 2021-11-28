#ifndef PYTHON_SCRIPT_H
#define PYTHON_SCRIPT_H

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include <map>
#include <string>

namespace MattEngine {

enum PropertyType { NUMBER, STRING };

class PythonScript {

public:
	PythonScript(const std::string& file);
	void onUpdate(float deltaTime);
	float getFloatProperty(const std::string& name);
	std::string getStringProperty(const std::string& name);
	void setFloatProperty(const std::string& name, float value);
	void setStringProperty(const std::string& name, const std::string& value);

public:
	std::map<std::string, PropertyType> PropertyTypesByName;

private:
	inline static bool s_loaded = false;

private:
	PyObject* m_module;
	PyObject* m_onUpdateFunction;
};

} // namespace MattEngine

#endif