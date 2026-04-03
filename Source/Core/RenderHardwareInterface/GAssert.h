#pragma once
#include "Result.h"
#include <string>

#ifdef _DEBUG

#define GRAPHICS_LOG_RESULT(code, category, desc, ...) \
	{ \
		::graphics::result::Message rMsg__ = ::graphics::result::GetErrorMessage(code); \
		LOG_ERROR(category, std::string(rMsg__.Desc) + " " + desc, __VA_ARGS__); \
	}

#define GRAPHICS_LOG_CODE(code, category, desc, ...) \
	LOG_ERROR(category, std::string(code ## _DESC) + " " + (desc), __VA_ARGS__)


#define GRAPHICS_ASSERT(expression, category, code, desc, ...)		\
if (!(bool)(expression))  											\
{  																	\
	GRAPHICS_LOG_CODE(code, category, desc, __VA_ARGS__);			\
	throw std::exception(desc);										\
}

#else

#define GRAPHICS_LOG_RESULT(code, category, desc, ...) void()
#define GRAPHICS_ASSERT(expression, category, code, desc, ...) void()

#endif

#define GRAPHICS_ASSERT_RESULT(code, category, desc, ...) \
if (GRAPHICS_FAILED(code)) \
{ \
	GRAPHICS_LOG_RESULT(code, category, desc, __VA_ARGS__); \
	return code; \
}