#include "PythonScript.h"

#include "EnginePythonModule.h"
#include "Log.h"

using namespace MattEngine;

void PythonScript::init() {
	PyImport_AppendInittab("mattengine", &initEnginePythonModule);
	Py_Initialize();
	PyRun_SimpleString("import sys");
}

PythonScript::PythonScript(const std::string& file) {
	if (!s_loaded) {
		init();
		s_loaded = true;
	}

	auto lastSlashIndex = file.find_last_of("/\\");
	std::string directory = file.substr(0, lastSlashIndex);
	std::string addDirectoryCommand = "sys.path.append(\"" + directory + "\")";
	PyRun_SimpleString(addDirectoryCommand.c_str());

	auto extensionIndex = file.find(".py");
	std::string fileName =
		file.substr(lastSlashIndex + 1, extensionIndex - lastSlashIndex - 1);

	PyObject* pythonModuleName = PyUnicode_FromString(fileName.c_str());
	m_module = PyImport_Import(pythonModuleName);
	Py_DECREF(pythonModuleName);

	if (m_module == NULL) {
		PyErr_Print();
		MATTENGINE_ASSERT(
			false, "Failed to load Python script: %s", file.c_str());
	}

	m_onUpdateFunction = PyObject_GetAttrString(m_module, "onUpdate");

	if (!m_onUpdateFunction || !PyCallable_Check(m_onUpdateFunction)) {
		MATTENGINE_ASSERT(
			false, "Failed to find Python function: onUpdate", NULL);
	}
}

void PythonScript::onUpdate(float deltaTime) {
	PyObject* args = PyTuple_New(1);
	PyTuple_SetItem(args, 0, PyFloat_FromDouble(deltaTime));
	PyObject_CallObject(m_onUpdateFunction, args);
	Py_DECREF(args);
}