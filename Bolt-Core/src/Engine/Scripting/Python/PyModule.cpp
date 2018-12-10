#include "Types.h"
#include "Types.h"
#include "PyModule.h"

namespace Bolt
{

	PyModule::PyModule() : PyModule("")
	{
	
	}

	PyModule::PyModule(const char* name)
		: m_Name(name), m_Methods(), m_ModuleDef()
	{
	
	}

	const PyMethods& PyModule::Methods() const
	{
		return m_Methods;
	}

	PyMethods& PyModule::Methods()
	{
		return m_Methods;
	}

	const char* PyModule::Name() const
	{
		return m_Name;
	}

	PyModuleDef& PyModule::CreateModuleDefinition()
	{
		m_ModuleDef = { PyModuleDef_HEAD_INIT, Name(), nullptr, -1, (PyMethodDef*)Methods().MethodDefs().data() };
		return m_ModuleDef;
	}

}