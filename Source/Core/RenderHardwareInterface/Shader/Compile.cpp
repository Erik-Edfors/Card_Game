#include "RenderHardwareInterface.pch.h"
#include "Compile.h"

#include <d3dcompiler.h>
#include <wrl.h>
#include <fstream>

#define FILE_CATEGORY "Compile.cpp"

namespace graphics
{
	static bool LoadStreamIntoMemory(void** outBuffer, size_t* outSize, std::istream& aStream, size_t aInitialBufferSize, size_t aMaxSize)
	{
		size_t bufferSize = aInitialBufferSize;	//1 MB
		size_t cursor = 0;
		char* buffer = new char[bufferSize];

		do
		{
			if (cursor == bufferSize)
			{
				bufferSize = bufferSize << 1;
				if (bufferSize > aMaxSize)
				{
					delete[] buffer;
					return false;
				}
				char* newBuffer = new char[bufferSize];
				memcpy(newBuffer, buffer, cursor);
				delete[] buffer;
				buffer = newBuffer;
			}

			aStream.read(&buffer[cursor], bufferSize - cursor);

			cursor += aStream.gcount();

		} while (aStream.eof() == false);

		*outBuffer = buffer;
		*outSize = cursor;
		return true;
	}

	static bool LoadFileIntoMemory(void** outBuffer, size_t* outSize, const wchar_t* aFilePath, size_t aInitialBufferSize, size_t aMaxSize)
	{
		std::ifstream file(aFilePath, std::ios::binary);

		if (file.is_open() == false)
		{
			return false;
		}

		return LoadStreamIntoMemory(outBuffer, outSize, file, aInitialBufferSize, aMaxSize);
	}

	static void ReleaseFromMemory(void* aBuffer)
	{
		delete[] aBuffer;
	}

	bool graphics::CompileShader(const char* aVersion, const std::filesystem::path& aFile, const char* aEntryPoint, void** outShaderCode, size_t* outShaderCodeSize)
	{
#define LOG_CATEGORY FILE_CATEGORY"->CompileShader"
#define CREATE_ERROR_MSG(details) "Failed to compile shader '%s'! " details

		if (outShaderCode == nullptr)
		{
			LOG_ERROR(LOG_CATEGORY, CREATE_ERROR_MSG(STRINGIFY(outShaderCode) " was nullptr."), aFile.string());
			return false;
		}
		if (outShaderCodeSize == nullptr)
		{
			LOG_ERROR(LOG_CATEGORY, CREATE_ERROR_MSG(STRINGIFY(outShaderCodeSize) " was nullptr."), aFile.string());
			return false;
		}

		UINT compileFlags = NULL;
#ifdef _DEBUG
		compileFlags |= D3DCOMPILE_DEBUG;
		compileFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
		compileFlags |= D3DCOMPILE_OPTIMIZATION_LEVEL0;
		compileFlags |= D3DCOMPILE_WARNINGS_ARE_ERRORS;
#else
		compileFlags |= D3DCOMPILE_SKIP_VALIDATION;
		compileFlags |= D3DCOMPILE_OPTIMIZATION_LEVEL3;
#endif

		Microsoft::WRL::ComPtr<ID3DBlob> code;
		Microsoft::WRL::ComPtr<ID3DBlob> error;
		{
			void* buffer = nullptr;
			size_t bufferSize = 0;
			LoadFileIntoMemory(&buffer, &bufferSize, aFile.c_str(), 65'536, 10'485'760);
			HRESULT result = D3DCompile(
				buffer,
				static_cast<SIZE_T>(bufferSize),
				aFile.string().c_str(),
				NULL,
				D3D_COMPILE_STANDARD_FILE_INCLUDE,
				aEntryPoint,
				aVersion,
				compileFlags,
				NULL,
				&code,
				&error
			);
			ReleaseFromMemory(buffer);
			if (FAILED(result))
			{
				if (error)
				{
					std::string errorMsg(reinterpret_cast<const char*>(error->GetBufferPointer()));
					LOG_ERROR(LOG_CATEGORY, CREATE_ERROR_MSG("%s"), aFile.string(), errorMsg);
				}
				else
				{
					LOG_ERROR(LOG_CATEGORY, CREATE_ERROR_MSG("An error happened, but failed to be determined."), aFile.string());
				}
				return false;
			}
		}

		const size_t bufferSize = code->GetBufferSize();
		if (bufferSize == 0)
		{
			LOG_ERROR(LOG_CATEGORY, CREATE_ERROR_MSG("No code was generated."), aFile.string());
			return false;
		}

		*outShaderCode = malloc(bufferSize);
		*outShaderCodeSize = bufferSize;

		memcpy(*outShaderCode, code->GetBufferPointer(), bufferSize);

		return true;

#undef LOG_CATEGORY
	}

	void ReleaseShaderCode(void* aShaderCode)
	{
		free(aShaderCode);
	}
}

#undef FILE_CATEGORY