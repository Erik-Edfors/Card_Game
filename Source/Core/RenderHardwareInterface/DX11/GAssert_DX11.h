#pragma once

#if _DEBUG
#define GRAPHICS_LOG_WIN32_ERROR(code, category) \
{ \
	char win32_msg_buffer__[1024]; \
	if (GRAPHICS_SUCCEEDED(::graphics::dx11::FormatWin32ErrorMessage(win32_msg_buffer__, sizeof(win32_msg_buffer__), (DWORD)(code)))) \
	{ \
		LOG_ERROR(std::string(category) + "->System Error", win32_msg_buffer__); \
	} \
}

#define GRAPHICS_LOG_FAILED_WIN32_API_CALL(code, signature, category) \
LOG_ERROR(category, ::std::string(::graphics::result::SYSTEM_ERROR_DESC) + (" Call to " #signature " failed with code '0x%x'!"), (DWORD)(code)); \
GRAPHICS_LOG_WIN32_ERROR(code, category)

#define GRAPHICS_ASSERT_WIN32_API_CALL(code, signature, category) \
if ((DWORD)(code) != NO_ERROR) \
{ \
	GRAPHICS_LOG_FAILED_WIN32_API_CALL(code, signature, category); \
	throw std::exception("Failed call to " #signature); \
}

// Old define when no exception was thrown.
//#define GRAPHICS_ASSERT_WIN32_API_CALL(code, signature, category) \
//if ((DWORD)(code) != NO_ERROR) \
//{ \
//	GRAPHICS_LOG_FAILED_WIN32_API_CALL(code, signature, category); \
//	return ::graphics::result::SYSTEM_ERROR; \
//}
#else
#define GRAPHICS_LOG_WIN_ERROR(code, category) void()
#define GRAPHICS_LOG_FAILED_WIN32_API_CALL(code, signature, category) void()
#define GRAPHICS_ASSERT_WIN32_API_CALL(code, signature, category) void()
#endif

namespace graphics
{
	namespace dx11
	{
		int FormatWin32ErrorMessage(char* outBuffer, size_t aBufferSize, DWORD aCode);
	}
}