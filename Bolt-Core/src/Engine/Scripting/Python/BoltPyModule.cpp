#include "bltpch.h"
#include "BoltPyModule.h"

#include "Core/Time/Time.h"

#define BLT_PY_METHOD_DEF(name) static PyObject* name(PyObject* self, PyObject* args)

namespace Bolt
{

	// =======================================================================================================================================
	// VECTOR FUNCTIONS
	// =======================================================================================================================================

	BLT_PY_METHOD_DEF(Bolt_AddVector)
	{
		return nullptr;
	}

	BLT_PY_METHOD_DEF(Bolt_NormalizeVector)
	{
		return nullptr;
	}

	// =======================================================================================================================================
	// TIME FUNCTIONS
	// =======================================================================================================================================

	BLT_PY_METHOD_DEF(Bolt_GetCurrentRealTime)
	{
		return PyFloat_FromDouble(Time::Get().RenderingTimeline().CurrentRealTime());
	}

	BLT_PY_METHOD_DEF(Bolt_GetCurrentTime)
	{
		return PyFloat_FromDouble(Time::Get().RenderingTimeline().CurrentTime());
	}

	// =======================================================================================================================================
	// MODULE DEFINITION / CREATION
	// =======================================================================================================================================

	static PyMethodDef BoltMethods[] = {
		// Time Methods
		{ "GetCurrentRealTime", Bolt_GetCurrentRealTime, METH_NOARGS, "Return the real current time that the Engine has been running for." },
		{ "GetCurrentTime", Bolt_GetCurrentTime, METH_NOARGS, "Return the current time that the Engine has been running for." },
		{ NULL, NULL, 0, NULL }
	};

	static struct PyModuleDef BoltModule = {
		PyModuleDef_HEAD_INIT,
		"Bolt",		/* name of module */
		NULL,		/* module documentation, may be NULL */
		-1,			/* size of per-interpreter state of the module, or -1 if the module keeps state in global variables. */
		BoltMethods
	};

	PyMODINIT_FUNC PyInit_Bolt()
	{
		return PyModule_Create(&BoltModule);
	}

}