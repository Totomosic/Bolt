#pragma once
#include "Logging.h"

inline blt::string ConvertError(GLenum error)
{
	switch (error)
	{
	case 0x0500: return "GL_INVALID_ENUM";
	case 0x0501: return "GL_INVALID_VALUE";
	case 0x0502: return "GL_INVALID_OPERATION";
	case 0x0503: return "GL_STACK_OVERFLOW";
	case 0x0504: return "GL_STACK_UNDERFLOW";
	case 0x0505: return "GL_OUT_OF_MEMORY";
	case 0x0506: return "GL_INVALID_FRAMEBUFFER_OPERATION";
	case 0x0507: return "GL_CONTEXT_LOST";
	case 0x8031: return "GL_TABLE_TOO_LARGE";
	}
	return "Unknown error";
}

#ifdef BLT_DEBUG
	#define GL_CALL(call) call; { int result = glGetError(); if (result != GL_NO_ERROR) { BLT_ASSERT(false, "OPENGL ERROR : " + ConvertError(result) + " (" + std::to_string(result) + ")"); } }
#else
	#define GL_CALL(call) call;
#endif

#define BLT_OFFSET_OF(object, field) ((size_t)(&(((object*)(nullptr))->field)))
#define BLT_BIT(num) (1 << num)
#define BLT_IS_BIT_SET(var, num) (!!((var) & BLT_BIT(num)))
#define BLT_CONTAINS_BIT(var, bitvalue) (!!(((int)var) & ((int)bitvalue)))

// Serialization Helpers
#define BLT_TRANSFER(backend, member_var) backend.Transfer(#member_var, &member_var)
#define BLT_TRANSFER_ARRAY(backend, member_var, length) backend.TransferArray(#member_var, member_var, length)