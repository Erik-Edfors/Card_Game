#pragma once

#define GRAPHICS_SUCCEEDED(code) (!!((::graphics::result::Code)(code) == ::graphics::result::OK))
#define GRAPHICS_FAILED(code) (!!((::graphics::result::Code)(code) != ::graphics::result::OK))

namespace graphics
{
	namespace result
	{
		typedef int Code;

		typedef struct
		{
			const char* Type;
			const char* Desc;
		} Message;

		Message GetErrorMessage(Code aCode);

#define DEFINE_ERROR(name, value, desc) \
constexpr Code name = (Code)(value); \
constexpr const char* name ## _DESC = desc

		DEFINE_ERROR(OK,						0,		"Succeeded!");
		DEFINE_ERROR(UNKNOWN,					1,		"Unknown Result!");
		DEFINE_ERROR(UNKNOWN_CODE,				2,		"Unknown Code!");

		//Logic Errors [100-199]
		DEFINE_ERROR(OUT_OF_RANGE,				101,	"Value Out Of Range!");
		DEFINE_ERROR(INVALID_ARGUMENT,			102,	"Invalid Argument!");
		DEFINE_ERROR(INVALID_IMPLEMENTATION,	103,	"Invalid Implementation!");

		//Runtime Errors [200-299]
		DEFINE_ERROR(INVALID_STATE,				201,	"Invalid State!");
		DEFINE_ERROR(SYSTEM_ERROR,				202,	"System Error!");
		DEFINE_ERROR(UNABLE_TO_OPEN_FILE,		203,	"Unable to open file!");

#undef DEFINE_ERROR
	}
}
