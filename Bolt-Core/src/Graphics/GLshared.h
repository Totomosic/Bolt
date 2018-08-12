#pragma once
#include "Bolt-Core.h"

namespace Bolt
{

	class BLT_API GLshared
	{
	public:
		GLshared() = default;
		GLshared(const GLshared& other) = delete;
		GLshared(GLshared&& other) = delete;
		GLshared& operator=(const GLshared& other) = delete;
		GLshared& operator=(GLshared&& other) = delete;

		virtual ~GLshared() {};

	};

}