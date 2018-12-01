#pragma once
#include "Bolt-Core.h"

namespace Bolt
{

	class BLT_API PyMethods
	{
	public:
		using MethodFunc = PyObject*(*)(PyObject*, PyObject*);

		BLT_API enum class CallConvention
		{
			Default = METH_VARARGS,
			Kwargs = METH_KEYWORDS
		};

		struct BLT_API PyMethodInfo
		{
		public:
			const char* Name;
			const char* Desc;
			MethodFunc Method;
			CallConvention Convention;
		};

	private:
		std::vector<PyMethodInfo> m_MethodInfos;
		std::vector<PyMethodDef> m_Methods;

	public:
		PyMethods();
		
		const std::vector<PyMethodInfo>& Methods() const;
		const std::vector<PyMethodDef>& MethodDefs() const;

		void AddMethod(const char* name, const MethodFunc& method, CallConvention convention = CallConvention::Default, const char* desc = "");

	};

	PyMethods::CallConvention operator|(PyMethods::CallConvention l, PyMethods::CallConvention r);

}