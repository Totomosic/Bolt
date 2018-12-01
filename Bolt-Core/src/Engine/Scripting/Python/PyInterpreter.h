#pragma once
#include "PyModule.h"
#include "PyScript.h"

namespace Bolt
{

	class BLT_API PyInterpreter
	{
	public:
		static constexpr size_t MAX_MODULES = 5;

	private:
		static PyModule s_Modules[MAX_MODULES];
		static PyModuleDef s_ModuleDefinitions[MAX_MODULES];
		static size_t s_UsedModules;
		static bool s_IsInitialized;

	public:
		PyInterpreter() = delete;

		static bool IsInitialized();

		static void Initialize();
		static void Terminate();

		static void AddModule(const PyModule& pyModule);
		static bool Execute(const PyScript& script);

	public:
		static PyObject* __CreateModule0();
		static PyObject* __CreateModule1();
		static PyObject* __CreateModule2();
		static PyObject* __CreateModule3();
		static PyObject* __CreateModule4();

	};

}