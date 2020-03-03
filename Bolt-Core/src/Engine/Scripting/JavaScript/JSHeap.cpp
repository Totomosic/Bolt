#include "bltpch.h"
#include "JSHeap.h"

namespace Bolt
{

	static void* bolt_duk_malloc(void* udata, size_t size)
	{
		return malloc(size);
	}

	static void* bolt_duk_realloc(void* udata, void* ptr, size_t size)
	{
		return realloc(ptr, size);
	}

	static void bolt_duk_free(void* udata, void* ptr)
	{
		free(ptr);
	}

	static void bolt_duk_fatal(void* udata, const char* message)
	{
		BLT_FATAL(message);
		abort();
	}

	JSHeap::JSHeap()
		: m_Context(duk_create_heap(
			bolt_duk_malloc,
			bolt_duk_realloc,
			bolt_duk_free,
			nullptr,
			bolt_duk_fatal
		))
	{
		
	}
	
	JSHeap::JSHeap(JSHeap&& other) noexcept
		: m_Context(other.m_Context)
	{
		other.m_Context = nullptr;
	}

	JSHeap& JSHeap::operator=(JSHeap&& other) noexcept
	{
		duk_context* ctx = m_Context;
		m_Context = other.m_Context;
		other.m_Context = ctx;
		return *this;
	}

	JSHeap::~JSHeap()
	{
		if (m_Context != nullptr)
		{
			duk_destroy_heap(m_Context);
		}
	}

	duk_context* JSHeap::GetNativeHeap() const
	{
		return m_Context;
	}

}
