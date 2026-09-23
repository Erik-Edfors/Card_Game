#pragma once
#include "../Device.h"
#include "../GraphicsDefs.h"
#include <filesystem>

namespace graphics
{
	class Data;

	//class VertexShader;
	//class HullShader;
	//class DomainShader;
	//class GeometryShader;
	//class PixelShader;
	//class ComputeShader;

	bool CompileShader(const char* aVersion, const std::filesystem::path& aFile, const char* aEntryPoint, void** outShaderCode, size_t* outShaderCodeSize);
	void ReleaseShaderCode(void* aShaderCode);

	//bool CompileShader(graphics::Device* aDevice, const std::filesystem::path& aFile, const char* aEntryPoint, graphics::VertexShader** outShader);
	//bool CompileShader(graphics::Device* aDevice, const std::filesystem::path& aFile, const char* aEntryPoint, graphics::HullShader** outShader);
	//bool CompileShader(graphics::Device* aDevice, const std::filesystem::path& aFile, const char* aEntryPoint, graphics::DomainShader** outShader);
	//bool CompileShader(graphics::Device* aDevice, const std::filesystem::path& aFile, const char* aEntryPoint, graphics::GeometryShader** outShader);
	//bool CompileShader(graphics::Device* aDevice, const std::filesystem::path& aFile, const char* aEntryPoint, graphics::PixelShader** outShader);
	//bool CompileShader(graphics::Device* aDevice, const std::filesystem::path& aFile, const char* aEntryPoint, graphics::ComputeShader** outShader);
}
