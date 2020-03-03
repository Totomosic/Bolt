#include "bltpch.h"
#include "JSContext.h"
#include "JSConsole.h"

namespace Bolt
{

	JSContext::JSContext(JSHeap* heap)
		: m_Heap(heap), m_Context(nullptr)
	{
		duk_push_thread(m_Heap->GetNativeHeap());
		m_Context = duk_get_context(m_Heap->GetNativeHeap(), -1);
		JSConsole::AddToContext(*this);
	}

	JSContext::JSContext(JSContext&& other) noexcept
		: m_Heap(other.m_Heap), m_Context(other.m_Context)
	{
		other.m_Context = nullptr;
	}

	JSContext& JSContext::operator=(JSContext&& other) noexcept
	{
		duk_context* ctx = m_Context;
		m_Heap = other.m_Heap;
		m_Context = other.m_Context;
		other.m_Context = ctx;
		return *this;
	}

	JSHeap* JSContext::GetHeap() const
	{
		return m_Heap;
	}

	duk_context* JSContext::GetNativeContext() const
	{
		return m_Context;
	}

	bool JSContext::ExecuteString(const std::string& code) const
	{
		if (duk_peval_string(GetNativeContext(), code.c_str()) != 0)
		{
			BLT_ERROR("JavaScript Error Occurred: {}", duk_safe_to_string(GetNativeContext(), -1));
			return false;
		}
		return true;
	}

}
