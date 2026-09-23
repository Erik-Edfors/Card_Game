#include "RenderHardwareInterface.pch.h"
#include "Compile.h"
//#include <Core/RenderHardwareInterface/PImpl.hpp>
//#include <Core/RenderHardwareInterface/Objects/Shaders/Shader.h>
//#include <Core/RenderHardwareInterface/Objects/Data.h>
#include <d3dcompiler.h>
#include <wrl.h>

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
		if (outShaderCode == nullptr)
		{
			//TODO: log
			return false;
		}
		if (outShaderCodeSize == nullptr)
		{
			//TODO: log
			return false;
		}

		UINT compileFlags = NULL;
#ifdef _DEBUG
		compileFlags |= D3DCOMPILE_DEBUG;
		compileFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
		compileFlags |= D3DCOMPILE_OPTIMIZATION_LEVEL0;
		compileFlags |= D3DCOMPILE_WARNINGS_ARE_ERRORS;
#elif
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
				//TODO: Log
				if (error)
				{
					std::string errorMsg(reinterpret_cast<const char*>(error->GetBufferPointer()));
					errorMsg;
				}
				return false;
			}
		}

		const size_t bufferSize = code->GetBufferSize();
		if (bufferSize == 0)
		{
			//TODO: Log
			return false;
		}

		*outShaderCode = malloc(bufferSize);
		*outShaderCodeSize = bufferSize;

		memcpy(*outShaderCode, code->GetBufferPointer(), bufferSize);

		return true;
	}

	void ReleaseShaderCode(void* aShaderCode)
	{
		free(aShaderCode);
	}
}
//
//static bool CompileShader(const std::filesystem::path& aFile, const char* aEntryPoint, ID3DBlob** outShader, const char* aShaderVersion)
//{
//    UINT compileFlags = NULL;
//#ifdef _DEBUG
//    compileFlags |= D3DCOMPILE_DEBUG;
//    compileFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
//    compileFlags |= D3DCOMPILE_OPTIMIZATION_LEVEL0;
//    compileFlags |= D3DCOMPILE_WARNINGS_ARE_ERRORS;
//#elif
//    compileFlags |= D3DCOMPILE_OPTIMIZATION_LEVEL3;
//#endif
//
//    void* buffer = nullptr;
//    size_t bufferSize = 0;
//    LoadFileIntoMemory(&buffer, &bufferSize, aFile.c_str(), 65'536, 10'485'760);
//    Microsoft::WRL::ComPtr<ID3DBlob> code;
//    Microsoft::WRL::ComPtr<ID3DBlob> error;
//    HRESULT result = D3DCompile(
//        buffer, 
//        static_cast<SIZE_T>(bufferSize), 
//        aFile.string().c_str(), 
//        NULL, 
//        D3D_COMPILE_STANDARD_FILE_INCLUDE, 
//        aEntryPoint,
//        aShaderVersion, 
//        compileFlags, 
//        NULL, 
//        outShader,
//        &error
//    );
//    ReleaseFromMemory(buffer);
//    if (FAILED(result))
//    {
//        //TODO: Log
//        return false;
//    }
//
//    return true;
//}
//
//bool CompileShader(graphics::Device* aDevice, const std::filesystem::path& aFile, const char* aEntryPoint, graphics::VertexShader** outShader)
//{
//    const std::string name = aFile.filename().replace_extension().string();
//    Microsoft::WRL::ComPtr<ID3DBlob> shader;
//    bool result = CompileShader(aFile, aEntryPoint, &shader, "vs_5_0");
//    if (not result)
//    {
//        return false;
//    }
//    return GRAPHICS_SUCCEEDED(aDevice->CreateShader(outShader, name.c_str(), shader->GetBufferPointer(), shader->GetBufferSize()));
//}
//
//bool CompileShader(graphics::Device* aDevice, const std::filesystem::path& aFile, const char* aEntryPoint, graphics::HullShader** outShader)
//{
//    const std::string name = aFile.filename().replace_extension().string();
//    Microsoft::WRL::ComPtr<ID3DBlob> shader;
//    bool result = CompileShader(aFile, aEntryPoint, &shader, "hs_5_0");
//    if (not result)
//    {
//        return false;
//    }
//    return GRAPHICS_SUCCEEDED(aDevice->CreateShader(outShader, name.c_str(), shader->GetBufferPointer(), shader->GetBufferSize()));
//}
//
//bool CompileShader(graphics::Device* aDevice, const std::filesystem::path& aFile, const char* aEntryPoint, graphics::DomainShader** outShader)
//{
//    const std::string name = aFile.filename().replace_extension().string();
//    Microsoft::WRL::ComPtr<ID3DBlob> shader;
//    bool result = CompileShader(aFile, aEntryPoint, &shader, "ds_5_0");
//    if (not result)
//    {
//        return false;
//    }
//    return GRAPHICS_SUCCEEDED(aDevice->CreateShader(outShader, name.c_str(), shader->GetBufferPointer(), shader->GetBufferSize()));
//}
//
//bool CompileShader(graphics::Device* aDevice, const std::filesystem::path& aFile, const char* aEntryPoint, graphics::GeometryShader** outShader)
//{
//    const std::string name = aFile.filename().replace_extension().string();
//    Microsoft::WRL::ComPtr<ID3DBlob> shader;
//    bool result = CompileShader(aFile, aEntryPoint, &shader, "gs_5_0");
//    if (not result)
//    {
//        return false;
//    }
//    return GRAPHICS_SUCCEEDED(aDevice->CreateShader(outShader, name.c_str(), shader->GetBufferPointer(), shader->GetBufferSize()));
//}
//
//bool CompileShader(graphics::Device* aDevice, const std::filesystem::path& aFile, const char* aEntryPoint, graphics::PixelShader** outShader)
//{
//    const std::string name = aFile.filename().replace_extension().string();
//    Microsoft::WRL::ComPtr<ID3DBlob> shader;
//    bool result = CompileShader(aFile, aEntryPoint, &shader, "ps_5_0");
//    if (not result)
//    {
//        return false;
//    }
//    return GRAPHICS_SUCCEEDED(aDevice->CreateShader(outShader, name.c_str(), shader->GetBufferPointer(), shader->GetBufferSize()));
//}
//
//bool CompileShader(graphics::Device* aDevice, const std::filesystem::path& aFile, const char* aEntryPoint, graphics::ComputeShader** outShader)
//{
//    const std::string name = aFile.filename().replace_extension().string();
//    Microsoft::WRL::ComPtr<ID3DBlob> shader;
//    bool result = CompileShader(aFile, aEntryPoint, &shader, "cs_5_0");
//    if (not result)
//    {
//        return false;
//    }
//    return GRAPHICS_SUCCEEDED(aDevice->CreateShader(outShader, name.c_str(), shader->GetBufferPointer(), shader->GetBufferSize()));
//}
