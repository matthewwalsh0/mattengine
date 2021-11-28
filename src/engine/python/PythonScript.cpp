#include "PythonScript.h"

#include "EnginePythonModule.h"
#include "Log.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>

using namespace MattEngine;

static void init() {
	PyImport_AppendInittab("mattengine", &initEnginePythonModule);
	Py_Initialize();
	PyRun_SimpleString("import sys");
}

static std::map<std::string, PropertyType> getPropertyTypesByName(
	const std::string& file) {
	std::ifstream fileStream(file);
	bool isNumber = false;
	bool isString = false;
	std::map<std::string, PropertyType> propertyTypesByName;
	std::string line;

	while (getline(fileStream, line)) {
		std::string variableName = line.substr(0, line.find(" = "));

		if (isNumber) {
			propertyTypesByName[variableName] = PropertyType::NUMBER;
			isNumber = false;
		}

		if (isString) {
			propertyTypesByName[variableName] = PropertyType::STRING;
			isString = false;
		}

		if (variableName == "# number") {
			isNumber = true;
		} else if (variableName == "# string") {
			isString = true;
		}
	}

	return propertyTypesByName;
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

	PropertyTypesByName = getPropertyTypesByName(file);
}

void PythonScript::onUpdate(float deltaTime) {
	PyObject* args = PyTuple_New(1);
	PyTuple_SetItem(args, 0, PyFloat_FromDouble(deltaTime));
	PyObject_CallObject(m_onUpdateFunction, args);
	Py_DECREF(args);
}

float PythonScript::getFloatProperty(const std::string& name) {
	return PyFloat_AsDouble(PyObject_GetAttrString(m_module, name.c_str()));
}

std::string PythonScript::getStringProperty(const std::string& name) {
	return PyUnicode_AsUTF8(PyObject_GetAttrString(m_module, name.c_str()));
}

void PythonScript::setStringProperty(
	const std::string& name, const std::string& value) {
	PyObject_SetAttrString(
		m_module, name.c_str(), PyUnicode_FromString(value.c_str()));
}

void PythonScript::setFloatProperty(const std::string& name, float value) {
	PyObject_SetAttrString(m_module, name.c_str(), PyFloat_FromDouble(value));
}