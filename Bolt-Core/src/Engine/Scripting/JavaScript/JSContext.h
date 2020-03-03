#pragma once
#include "JSHeap.h"

namespace Bolt
{

	class BLT_API JSContext
	{
	private:
		JSHeap* m_Heap;
		duk_context* m_Context;

	public:
		JSContext(JSHeap* heap);
		JSContext(const JSContext& other) = delete;
		JSContext& operator=(const JSContext& other) = delete;
		JSContext(JSContext&& other) noexcept;
		JSContext& operator=(JSContext&& other) noexcept;

		JSHeap* GetHeap() const;
		duk_context* GetNativeContext() const;

		bool ExecuteString(const std::string& code) const;
	};

}