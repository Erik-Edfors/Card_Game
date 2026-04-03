#include "RenderHardwareInterface.pch.h"
#include "Result.h"

namespace graphics
{
	namespace result
	{
#define CODE_MESSAGE(code) (Message{ #code, code ## _DESC })
#define RESULT_CASE(code) case code ## : return CODE_MESSAGE(code)
#define RESULT_DEFAULT(code) default: return CODE_MESSAGE(code)

		Message GetErrorMessage(Code aCode)
		{
			switch (aCode)
			{
				RESULT_CASE(OK);
				RESULT_CASE(UNKNOWN);
				RESULT_CASE(UNKNOWN_CODE);

				RESULT_CASE(OUT_OF_RANGE);
				RESULT_CASE(INVALID_ARGUMENT);
				RESULT_CASE(INVALID_IMPLEMENTATION);

				RESULT_CASE(INVALID_STATE);
				RESULT_CASE(SYSTEM_ERROR);
				RESULT_CASE(UNABLE_TO_OPEN_FILE);

				RESULT_DEFAULT(UNKNOWN_CODE);
			}
		}
	}
}

