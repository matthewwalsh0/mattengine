#define PY_SSIZE_T_CLEAN

#include "Entity.h"
#include "TransformComponent.h"

#include <Python.h>

#include "structmember.h"

typedef struct {
	PyObject_HEAD MattEngine::Entity entity;
} PythonEntity;

static PyObject* pythonEntity_new(
	PyTypeObject* type, PyObject* args, PyObject* kwds) {
	PythonEntity* self = (PythonEntity*)type->tp_alloc(type, 0);
	return (PyObject*)self;
}

static void pythonEntity_dealloc(PythonEntity* self) {
	Py_TYPE(self)->tp_free((PyObject*)self);
}

static int pythonEntity_init(
	PythonEntity* self, PyObject* args, PyObject* kwds) {
	return 0;
}

static PyObject* pythonEntity_tag(
	PythonEntity* self, PyObject* Py_UNUSED(ignored)) {
	if (!self->entity.hasComponent<TagComponent>())
		return Py_None;

	std::string tag = self->entity.getComponent<TagComponent>().Tag;

	return PyUnicode_FromString(tag.c_str());
}

static PyObject* pythonEntity_position(
	PythonEntity* self, PyObject* Py_UNUSED(ignored)) {
	if (!self->entity.hasComponent<TransformComponent>())
		return Py_None;

	glm::vec3& position =
		self->entity.getComponent<TransformComponent>().Position;

	PyObject* result = PyTuple_New(3);
	PyTuple_SetItem(result, 0, PyFloat_FromDouble(position.x));
	PyTuple_SetItem(result, 1, PyFloat_FromDouble(position.y));
	PyTuple_SetItem(result, 2, PyFloat_FromDouble(position.z));

	return result;
}

static PyObject* pythonEntity_setPosition(PythonEntity* self, PyObject* args) {
	if (!self->entity.hasComponent<TransformComponent>())
		return Py_None;

	float x, y, z;

	if (!PyArg_ParseTuple(args, "fff", &x, &y, &z))
		return NULL;

	TransformComponent& transformComponent =
		self->entity.getComponent<TransformComponent>();

	transformComponent.Position.x = x;
	transformComponent.Position.y = y;
	transformComponent.Position.z = z;

	return Py_None;
}

static PyMemberDef pythonEntity_members[] = {{NULL}};

static PyMethodDef pythonEntity_methods[] = {
	{"tag", (PyCFunction)pythonEntity_tag, METH_NOARGS, ""},
	{"position", (PyCFunction)pythonEntity_position, METH_NOARGS, ""},
	{"set_position", (PyCFunction)pythonEntity_setPosition, METH_VARARGS},
	{NULL}};

static PyTypeObject PythonEntityType = {
	PyVarObject_HEAD_INIT(NULL, 0).tp_name = "mattengine.Entity",
	.tp_doc = "MattEngine Entity",
	.tp_basicsize = sizeof(PythonEntity),
	.tp_itemsize = 0,
	.tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
	.tp_new = pythonEntity_new,
	.tp_init = (initproc)pythonEntity_init,
	.tp_dealloc = (destructor)pythonEntity_dealloc,
	.tp_members = pythonEntity_members,
	.tp_methods = pythonEntity_methods,
};