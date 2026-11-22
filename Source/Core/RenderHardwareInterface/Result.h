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

		constexpr Message GetErrorMessage(Code aCode);

#define GRAPHICS_DEFINE_ERROR(name, value, desc) \
constexpr Code name = (Code)(value); \
constexpr const char* name ## _DESC = desc

		GRAPHICS_DEFINE_ERROR(OK,						0,		"Succeeded!");
		GRAPHICS_DEFINE_ERROR(UNKNOWN,					1,		"Unknown Result!");
		GRAPHICS_DEFINE_ERROR(UNKNOWN_CODE,				2,		"Unknown Code!");

		//Logic Errors [100-199]
		GRAPHICS_DEFINE_ERROR(OUT_OF_RANGE,				101,	"Value Out Of Range!");
		GRAPHICS_DEFINE_ERROR(INVALID_ARGUMENT,			102,	"Invalid Argument!");
		GRAPHICS_DEFINE_ERROR(INVALID_IMPLEMENTATION,	103,	"Invalid Implementation!");

		//Runtime Errors [200-299]
		GRAPHICS_DEFINE_ERROR(INVALID_STATE,				201,	"Invalid State!");
		GRAPHICS_DEFINE_ERROR(SYSTEM_ERROR,				202,	"System Error!");
		GRAPHICS_DEFINE_ERROR(UNABLE_TO_OPEN_FILE,		203,	"Unable to open file!");

#undef GRAPHICS_DEFINE_ERROR

#define GRAPHICS_CODE_MESSAGE(code) (Message{ #code, code ## _DESC })
#define GRAPHICS_RESULT_CASE(code) case code ## : return GRAPHICS_CODE_MESSAGE(code)
#define GRAPHICS_RESULT_DEFAULT(code) default: return GRAPHICS_CODE_MESSAGE(code)

		inline constexpr Message GetErrorMessage(Code aCode)
		{
			switch (aCode)
			{
				GRAPHICS_RESULT_CASE(OK);
				GRAPHICS_RESULT_CASE(UNKNOWN);
				GRAPHICS_RESULT_CASE(UNKNOWN_CODE);

				GRAPHICS_RESULT_CASE(OUT_OF_RANGE);
				GRAPHICS_RESULT_CASE(INVALID_ARGUMENT);
				GRAPHICS_RESULT_CASE(INVALID_IMPLEMENTATION);

				GRAPHICS_RESULT_CASE(INVALID_STATE);
				GRAPHICS_RESULT_CASE(SYSTEM_ERROR);
				GRAPHICS_RESULT_CASE(UNABLE_TO_OPEN_FILE);

				GRAPHICS_RESULT_DEFAULT(UNKNOWN_CODE);
			}
		}

#undef CODE_MESSAGE
#undef RESULT_CASE
#undef RESULT_DEFAULT
	}
}
