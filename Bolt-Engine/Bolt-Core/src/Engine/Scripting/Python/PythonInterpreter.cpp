#include "bltpch.h"
#include "PythonInterpreter.h"
#include "BoltPyModule.h"

#include "Core/Utils/Filesystem/Filesystem.h"

namespace Bolt
{

#define BLT_PATH_SEPARATOR ';'

	PyThreadState* PythonInterpreter::s_GlobalThread = nullptr;

	void PythonInterpreter::Initialize(const std::vector<blt::string>& libraryPaths)
	{
		blt::string path = "";
		for (const blt::string& s : libraryPaths)
		{
			path += s + BLT_PATH_SEPARATOR;
		}
		wchar_t* str = Py_DecodeLocale(path.c_str(), nullptr);
		Py_SetPath(str);
		PyMem_RawFree(str);

		PyImport_AppendInittab("Bolt", PyInit_Bolt); 

		Py_Initialize();
		PyEval_InitThreads();
		s_GlobalThread = PyThreadState_Get();
	}

	void PythonInterpreter::Cleanup()
	{
		Py_Finalize();
	}

	PythonInterpreter::PythonInterpreter()
		: m_State(Py_NewInterpreter())
	{
	
	}

	PythonInterpreter::PythonInterpreter(PythonInterpreter&& other)
		: m_State(other.m_State)
	{
		other.m_State = nullptr;
	}

	PythonInterpreter& PythonInterpreter::operator=(PythonInterpreter&& other)
	{
		PyThreadState* state = m_State;
		m_State = other.m_State;
		other.m_State = state;
		return *this;
	}

	PythonInterpreter::~PythonInterpreter()
	{
		if (m_State != nullptr)
		{
			Bind();
			Py_EndInterpreter(m_State);
			Unbind();
		}
	}

	void PythonInterpreter::Bind() const
	{
		PyThreadState_Swap(m_State);
	}

	void PythonInterpreter::Unbind() const
	{
		PyThreadState_Swap(s_GlobalThread);
	}

	bool PythonInterpreter::RunString(const blt::string& cmd) const
	{
		Bind();
		return PyRun_SimpleString(cmd.c_str()) == 0;
	}

	bool PythonInterpreter::RunFile(const Filepath& file) const
	{
		File f = Filesystem::Open(file, OpenMode::Read);
		return RunString(f.ReadText());
	}

	void PythonInterpreter::Reset()
	{
		if (m_State != nullptr)
		{
			Bind();
			Py_EndInterpreter(m_State);
			Unbind();
		}
		m_State = Py_NewInterpreter();
	}

}