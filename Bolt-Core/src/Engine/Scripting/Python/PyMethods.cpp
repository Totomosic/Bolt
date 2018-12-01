#include "Types.h"
#include "PyMethods.h"

namespace Bolt
{

	PyMethods::CallConvention operator|(PyMethods::CallConvention left, PyMethods::CallConvention right)
	{
		return (PyMethods::CallConvention)((int)left | (int)right);
	}

	PyMethods::PyMethods()
		: m_MethodInfos(), m_Methods{ { NULL, NULL, 0, NULL } }
	{
		
	}

	const std::vector<PyMethods::PyMethodInfo>& PyMethods::Methods() const
	{
		return m_MethodInfos;
	}

	const std::vector<PyMethodDef>& PyMethods::MethodDefs() const
	{
		return m_Methods;
	}

	void PyMethods::AddMethod(const char* name, const PyMethods::MethodFunc& method, PyMethods::CallConvention convention, const char* desc)
	{
		PyMethodInfo info;
		info.Name = name;
		info.Method = method;
		info.Convention = convention;
		info.Desc = desc;
		m_MethodInfos.insert(m_MethodInfos.begin(), info);
		m_Methods.insert(m_Methods.begin(), { name, method, (int)convention, desc });
	}

}