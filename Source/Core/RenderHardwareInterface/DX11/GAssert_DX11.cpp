#include "RenderHardwareInterface.pch.h"
#include "GAssert_DX11.h"

#undef FILE_CATEGORY
#undef CLASS_CATEGORY
#define FILE_CATEGORY "Error_DX11.cpp"
#define CLASS_CATEGORY FILE_CATEGORY"->"

int graphics::dx11::FormatWin32ErrorMessage(char* outBuffer, size_t aBufferSize, DWORD aCode)
{
#define LOG_CATEGORY CLASS_CATEGORY"::FormatWin32ErrorMessage"
    GRAPHICS_ASSERT(outBuffer != nullptr, LOG_CATEGORY, result::INVALID_ARGUMENT, "outBuffer was nullptr!");
    GRAPHICS_ASSERT(aBufferSize > 0, LOG_CATEGORY, result::INVALID_ARGUMENT, "aBufferSize was 0!");

    DWORD result = FormatMessageA(
        FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_MAX_WIDTH_MASK,
        NULL,
        aCode,
        NULL,
        (LPSTR)outBuffer,
        (DWORD)aBufferSize,
        NULL
    );

    GRAPHICS_ASSERT(result != 0, LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(FormatMessageA) " failed with code '0x%x'!", GetLastError());
    result;

    return result::OK;
#undef LOG_CATEGORY
}
