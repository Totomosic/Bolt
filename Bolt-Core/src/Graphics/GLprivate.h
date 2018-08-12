#pragma once
#include "Bolt-Core.h"

namespace Bolt
{

	class BLT_API GLprivate
	{
	public:
		GLprivate() = default;
		GLprivate(const GLprivate& other) = delete;
		GLprivate(GLprivate&& other) = delete;
		GLprivate& operator=(const GLprivate& other) = delete;
		GLprivate& operator=(GLprivate&& other) = delete;

		virtual ~GLprivate() {};

	};

}