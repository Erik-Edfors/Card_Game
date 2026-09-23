#include "ShaderRegistry.h"
#include <Core/RenderHardwareInterface/Shader/Compile.h>
#include <Core/RenderHardwareInterface/Objects/Shaders/Shader.h>
#include <Core/RenderHardwareInterface/Objects/Shaders/VertexShader.h>
#include <Core/RenderHardwareInterface/Objects/Shaders/HullShader.h>
#include <Core/RenderHardwareInterface/Objects/Shaders/DomainShader.h>
#include <Core/RenderHardwareInterface/Objects/Shaders/GeometryShader.h>
#include <Core/RenderHardwareInterface/Objects/Shaders/PixelShader.h>
#include <Core/RenderHardwareInterface/Objects/Shaders/ComputeShader.h>

assets::ShaderRegistry::ShaderRegistry(graphics::Device* aDevice) :
	myDevice(aDevice)
{
}

bool assets::ShaderRegistry::OnLoad(const std::filesystem::path& aPath, ShaderAsset* outResult)
{
	std::string fileName = aPath.filename().replace_extension().string();

	if (fileName.size() < 3)
	{
		//TODO: Log no shader type identifier
		return false;
	}

	std::string shaderVersion;
	switch (CommonUtilities::StringHash(fileName.substr(fileName.size() - 3)))
	{
	case "_VS"_hash: shaderVersion = "vs_5_0"; break;
	case "_HS"_hash: shaderVersion = "hs_5_0"; break;
	case "_DS"_hash: shaderVersion = "ds_5_0"; break;
	case "_GS"_hash: shaderVersion = "gs_5_0"; break;
	case "_PS"_hash: shaderVersion = "ps_5_0"; break;
	case "_CS"_hash: shaderVersion = "cs_5_0"; break;
	default:
		//TODO: Log unknown shader version.
		return false;
	}

	void* shaderCode = nullptr;
	size_t shaderCodeSize = 0;

	if (graphics::CompileShader(shaderVersion.c_str(), aPath, "main", &shaderCode, &shaderCodeSize))
	{
		outResult->ByteCodeSize = shaderCodeSize;
		outResult->ByteCode = std::make_unique<char[]>(outResult->ByteCodeSize);
		memcpy(outResult->ByteCode.get(), shaderCode, outResult->ByteCodeSize);

		switch (CommonUtilities::StringHash(shaderVersion.substr(0, 2)))
		{
		case "vs"_hash:
		{
			graphics::PImpl<graphics::VertexShader> shader;
			graphics::result::Code result = myDevice->CreateShader(&shader, fileName.c_str(), shaderCode, shaderCodeSize);
			graphics::ReleaseShaderCode(shaderCode);
			if (GRAPHICS_FAILED(result))
			{
				//TODO: Log
				return false;
			}
			return shader.As(&outResult->Shader);
		}
		case "hs"_hash:
		{
			graphics::PImpl<graphics::HullShader> shader;
			graphics::result::Code result = myDevice->CreateShader(&shader, fileName.c_str(), shaderCode, shaderCodeSize);
			graphics::ReleaseShaderCode(shaderCode);
			if (GRAPHICS_FAILED(result))
			{
				//TODO: Log
				return false;
			}
			return shader.As(&outResult->Shader);
		}
		case "ds"_hash:
		{
			graphics::PImpl<graphics::DomainShader> shader;
			graphics::result::Code result = myDevice->CreateShader(&shader, fileName.c_str(), shaderCode, shaderCodeSize);
			graphics::ReleaseShaderCode(shaderCode);
			if (GRAPHICS_FAILED(result))
			{
				//TODO: Log
				return false;
			}
			return shader.As(&outResult->Shader);
		}
		case "gs"_hash:
		{
			graphics::PImpl<graphics::GeometryShader> shader;
			graphics::result::Code result = myDevice->CreateShader(&shader, fileName.c_str(), shaderCode, shaderCodeSize);
			graphics::ReleaseShaderCode(shaderCode);
			if (GRAPHICS_FAILED(result))
			{
				//TODO: Log
				return false;
			}
			return shader.As(&outResult->Shader);
		}
		case "ps"_hash:
		{
			graphics::PImpl<graphics::PixelShader> shader;
			graphics::result::Code result = myDevice->CreateShader(&shader, fileName.c_str(), shaderCode, shaderCodeSize);
			graphics::ReleaseShaderCode(shaderCode);
			if (GRAPHICS_FAILED(result))
			{
				//TODO: Log
				return false;
			}
			return shader.As(&outResult->Shader);
		}
		case "cs"_hash:
		{
			graphics::PImpl<graphics::ComputeShader> shader;
			graphics::result::Code result = myDevice->CreateShader(&shader, fileName.c_str(), shaderCode, shaderCodeSize);
			graphics::ReleaseShaderCode(shaderCode);
			if (GRAPHICS_FAILED(result))
			{
				//TODO: Log
				return false;
			}
			return shader.As(&outResult->Shader);
		}
		}
	}

	return false;
}
