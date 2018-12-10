#include "Types.h"
#include "Types.h"
#include "PyInterpreter.h"

namespace Bolt
{

	PyModule PyInterpreter::s_Modules[PyInterpreter::MAX_MODULES];
	PyModuleDef PyInterpreter::s_ModuleDefinitions[PyInterpreter::MAX_MODULES];
	size_t PyInterpreter::s_UsedModules = 0;
	bool PyInterpreter::s_IsInitialized = false;

	bool PyInterpreter::IsInitialized()
	{
		return s_IsInitialized;
	}

	void PyInterpreter::Initialize()
	{
		switch (s_UsedModules)
		{
		case 0:
			return;
		case 1:
			PyImport_AppendInittab(s_Modules[0].Name(), &__CreateModule0);
			break;
		case 2:
			PyImport_AppendInittab(s_Modules[0].Name(), &__CreateModule0);
			PyImport_AppendInittab(s_Modules[1].Name(), &__CreateModule1);
			break;
		case 3:
			PyImport_AppendInittab(s_Modules[0].Name(), &__CreateModule0);
			PyImport_AppendInittab(s_Modules[1].Name(), &__CreateModule1);
			PyImport_AppendInittab(s_Modules[2].Name(), &__CreateModule2);
			break;
		case 4:
			PyImport_AppendInittab(s_Modules[0].Name(), &__CreateModule0);
			PyImport_AppendInittab(s_Modules[1].Name(), &__CreateModule1);
			PyImport_AppendInittab(s_Modules[2].Name(), &__CreateModule2);
			PyImport_AppendInittab(s_Modules[3].Name(), &__CreateModule3);
			break;
		case 5:
			PyImport_AppendInittab(s_Modules[0].Name(), &__CreateModule0);
			PyImport_AppendInittab(s_Modules[1].Name(), &__CreateModule1);
			PyImport_AppendInittab(s_Modules[2].Name(), &__CreateModule2);
			PyImport_AppendInittab(s_Modules[3].Name(), &__CreateModule3);
			PyImport_AppendInittab(s_Modules[4].Name(), &__CreateModule4);
			break;
		default:
			BLT_CORE_ERROR("Only 5 modules are supported");
			return;
		}
		s_IsInitialized = true;
		Py_Initialize();
	}

	void PyInterpreter::Terminate()
	{
		Py_Finalize();
	}

	void PyInterpreter::AddModule(const PyModule& pyModule)
	{
		if (!s_IsInitialized)
		{
			s_Modules[s_UsedModules] = pyModule;
			s_ModuleDefinitions[s_UsedModules++] = s_Modules[0].CreateModuleDefinition();
		}
		else
		{
			BLT_CORE_ERROR("Interpreter has already been initialized, add all modules before calling Intialize()");
		}
	}

	bool PyInterpreter::Execute(const PyScript& script)
	{
		if (!s_IsInitialized)
		{
			BLT_CORE_ERROR("Interpreter has not been initialized, call Initialize() before executing any scripts");
			return false;
		}
		PyRun_SimpleString(script.Source());
		return true;
	}

	PyObject* PyInterpreter::__CreateModule0()
	{
		return PyModule_Create(&s_ModuleDefinitions[0]);
	}

	PyObject* PyInterpreter::__CreateModule1()
	{
		return PyModule_Create(&s_Modules[1].CreateModuleDefinition());
	}

	PyObject* PyInterpreter::__CreateModule2()
	{
		return PyModule_Create(&s_Modules[2].CreateModuleDefinition());
	}

	PyObject* PyInterpreter::__CreateModule3()
	{
		return PyModule_Create(&s_Modules[3].CreateModuleDefinition());
	}

	PyObject* PyInterpreter::__CreateModule4()
	{
		return PyModule_Create(&s_Modules[4].CreateModuleDefinition());
	}

}