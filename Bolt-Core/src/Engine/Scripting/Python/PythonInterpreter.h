#pragma once
#include "bltpch.h"
#include "Python.h"

#include "Core/Utils/Filesystem/Filepath.h"

namespace Bolt
{

	class BLT_API PythonInterpreter
	{
	private:
		static PyThreadState* s_GlobalThread;

	public:
		static void Initialize(const std::vector<blt::string>& libraryPaths);
		static void Cleanup();

	private:
		PyThreadState* m_State;

	public:
		PythonInterpreter();
		PythonInterpreter(PythonInterpreter&& other);
		PythonInterpreter& operator=(PythonInterpreter&& other);
		~PythonInterpreter();

		void Bind() const;
		void Unbind() const;
		bool RunString(const blt::string& cmd) const;
		bool RunFile(const Filepath& file) const;

		void Reset();
	};

}