#include "EntityPythonType.h"
#include "Game.h"

#define PY_SSIZE_T_CLEAN
#include <Python.h>

using namespace MattEngine;

static PyObject* pythonEngine_getEntity(PyObject* self, PyObject* args) {
	const char* entityName;

	if (!PyArg_ParseTuple(args, "s", &entityName))
		return NULL;

	Scene& scene = Game::getInstance().getScene();
	auto entity = scene.getEntity(entityName);

	if (!entity)
		return Py_None;

	PyObject* entityInstance =
		PyObject_CallObject((PyObject*)&PythonEntityType, NULL);

	((PythonEntity*)entityInstance)->entity = *entity;

	return entityInstance;
}

static PyMethodDef pythonEngine_methods[] = {
	{"get_entity", pythonEngine_getEntity, METH_VARARGS,
		"Return the number of arguments received by the process."},
	{NULL, NULL, 0, NULL}};

static PyModuleDef pythonEngine_module = {PyModuleDef_HEAD_INIT, "mattengine",
	NULL, -1, pythonEngine_methods, NULL, NULL, NULL, NULL};

static PyObject* initEnginePythonModule(void) {
	if (PyType_Ready(&PythonEntityType) < 0)
		return NULL;

	PyObject* m = PyModule_Create(&pythonEngine_module);
	if (m == NULL)
		return NULL;

	Py_INCREF(&PythonEntityType);
	if (PyModule_AddObject(m, "Entity", (PyObject*)&PythonEntityType) < 0) {
		Py_DECREF(&PythonEntityType);
		Py_DECREF(m);
		return NULL;
	}

	return m;
}