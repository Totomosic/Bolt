#pragma once
#include "PyMethods.h"

namespace Bolt
{

	class BLT_API PyModule
	{
	public:
		using CreationFunction = PyObject*(*)(void);

	private:
		const char* m_Name;
		PyMethods m_Methods;
		PyModuleDef m_ModuleDef;

	public:
		PyModule();
		PyModule(const char* name);

		const PyMethods& Methods() const;
		PyMethods& Methods();
		const char* Name() const;
		PyModuleDef& CreateModuleDefinition();

	};

}