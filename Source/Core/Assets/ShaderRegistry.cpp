#include "Assets.pch.h"
#include "ShaderRegistry.h"
#include <Core/RenderHardwareInterface/Shader/Compile.h>
#include <Core/RenderHardwareInterface/Objects/Shaders/Shader.h>
#include <Core/RenderHardwareInterface/Objects/Shaders/VertexShader.h>
#include <Core/RenderHardwareInterface/Objects/Shaders/HullShader.h>
#include <Core/RenderHardwareInterface/Objects/Shaders/DomainShader.h>
#include <Core/RenderHardwareInterface/Objects/Shaders/GeometryShader.h>
#include <Core/RenderHardwareInterface/Objects/Shaders/PixelShader.h>
#include <Core/RenderHardwareInterface/Objects/Shaders/ComputeShader.h>

#define FILE_CATEGORY "ShaderRegistry.cpp"
#define CLASS_CATEGORY FILE_CATEGORY"->ShaderRegistry"

assets::ShaderRegistry::ShaderRegistry(graphics::Device* aDevice) :
	myDevice(aDevice)
{
}

bool assets::ShaderRegistry::OnLoad(const std::filesystem::path& aPath, ShaderAsset* outResult)
{
#define LOG_CATEGORY CLASS_CATEGORY"::OnLoad"
#define MAKE_ERROR_MSG(details) "Failed to load shader '%s'! " details

	std::string extension = aPath.extension().string();
	std::transform(
		extension.begin(),
		extension.end(),
		extension.begin(),
		[](char c)
		{
			return static_cast<char>(tolower(c));
		}
	);

	if (extension == ".hlsli")
	{
		LOG_INFO(LOG_CATEGORY, "Skipped file '%s'. File is a shader include file.", aPath.string());
		return false;
	}
	else if (extension != ".hlsl")
	{
		LOG_ERROR(LOG_CATEGORY, MAKE_ERROR_MSG("File must have '.hlsl' extension."), aPath.string());
		return false;
	}

	std::string fileName = aPath.filename().replace_extension().string();

	if (fileName.size() < 3)
	{
		LOG_ERROR(LOG_CATEGORY, MAKE_ERROR_MSG("Filename too short to contain shader type postfix."), aPath.string());
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
		LOG_ERROR(LOG_CATEGORY, MAKE_ERROR_MSG("Filename did not contain a shader type postfix."), aPath.string());
		return false;
	}

	void* shaderCode = nullptr;
	size_t shaderCodeSize = 0;

	if (graphics::CompileShader(shaderVersion.c_str(), aPath, "main", &shaderCode, &shaderCodeSize))
	{
		outResult->ByteCodeSize = shaderCodeSize;
		outResult->ByteCode = std::make_unique<char[]>(outResult->ByteCodeSize);
		memcpy(outResult->ByteCode.get(), shaderCode, outResult->ByteCodeSize);

		graphics::result::Code result = graphics::result::INVALID_ARGUMENT;
		graphics::PImpl<graphics::Shader> shader;

		switch (CommonUtilities::StringHash(shaderVersion.substr(0, 2)))
		{
		case "vs"_hash:
		{
			graphics::PImpl<graphics::VertexShader> vsShader;
			result = myDevice->CreateShader(&vsShader, fileName.c_str(), shaderCode, shaderCodeSize);
			vsShader.As(&outResult->Shader);
		}
		break;
		case "hs"_hash:
		{
			graphics::PImpl<graphics::HullShader> hsShader;
			result = myDevice->CreateShader(&hsShader, fileName.c_str(), shaderCode, shaderCodeSize);
			hsShader.As(&outResult->Shader);
		}
		break;
		case "ds"_hash:
		{
			graphics::PImpl<graphics::DomainShader> dsShader;
			result = myDevice->CreateShader(&dsShader, fileName.c_str(), shaderCode, shaderCodeSize);
			dsShader.As(&outResult->Shader);
		}
		break;
		case "gs"_hash:
		{
			graphics::PImpl<graphics::GeometryShader> gsShader;
			result = myDevice->CreateShader(&gsShader, fileName.c_str(), shaderCode, shaderCodeSize);
			gsShader.As(&outResult->Shader);
		}
		break;
		case "ps"_hash:
		{
			graphics::PImpl<graphics::PixelShader> psShader;
			result = myDevice->CreateShader(&psShader, fileName.c_str(), shaderCode, shaderCodeSize);
			psShader.As(&outResult->Shader);
		}
		break;
		case "cs"_hash:
		{
			graphics::PImpl<graphics::ComputeShader> csShader;
			result = myDevice->CreateShader(&csShader, fileName.c_str(), shaderCode, shaderCodeSize);
			csShader.As(&outResult->Shader);
		}
		break;
		}

		graphics::ReleaseShaderCode(shaderCode);

		if (GRAPHICS_FAILED(result))
		{
			graphics::result::Message msg = graphics::result::GetErrorMessage(result);
			LOG_ERROR(LOG_CATEGORY, MAKE_ERROR_MSG("%s: %s"), aPath.string(), msg.Type, msg.Desc);
			msg;
			return false;
		}

		return true;
	}

	return false;

#undef MAKE_ERROR_MSG
#undef LOG_CATEGORY
}

#undef FILE_CATEGORY
#undef CLASS_CATEGORY
