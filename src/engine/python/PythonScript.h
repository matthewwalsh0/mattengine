#ifndef PYTHON_SCRIPT_H
#define PYTHON_SCRIPT_H

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include <string>

namespace MattEngine {

class PythonScript {

public:
	static void init();

public:
	PythonScript(const std::string& file);
	void onUpdate(float deltaTime);

private:
	inline static bool s_loaded = false;

private:
	PyObject* m_module;
	PyObject* m_onUpdateFunction;
};

} // namespace MattEngine

#endif