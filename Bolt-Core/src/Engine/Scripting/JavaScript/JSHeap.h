#pragma once
#include "duktape.h"
#include "bltpch.h"

namespace Bolt
{

	class BLT_API JSHeap
	{
	private:
		duk_context* m_Context;

	public:
		JSHeap();
		JSHeap(const JSHeap& other) = delete;
		JSHeap& operator=(const JSHeap& other) = delete;
		JSHeap(JSHeap&& other) noexcept;
		JSHeap& operator=(JSHeap&& other) noexcept;
		~JSHeap();

		duk_context* GetNativeHeap() const;

	};

}