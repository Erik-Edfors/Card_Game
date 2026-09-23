#include "RenderHardwareInterface.pch.h"
#include "DDSLoad.h"
#include "Device_DX11Impl.h"
#include "Objects/Texture2D_DX11Impl.h"
#include "GAssert_DX11.h"
#include "Detail/DDSTextureLoader11.h"
#include "Common_DX11.h"
#include <Core/CommonUtilities/StringHelper.h>
#include <fstream>

#undef FILE_CATEGORY
#undef CLASS_CATEGORY
#define FILE_CATEGORY "DDSLoad_DX11Impl.cpp"
#define CLASS_CATEGORY FILE_CATEGORY"->"

graphics::result::Code graphics::detail::ReadFile(const wchar_t* aPath, char** outBuffer, size_t* outSize)
{
#define LOG_CATEGORY CLASS_CATEGORY"::ReadFile"

	std::ifstream file(aPath, std::ios::binary);

	GRAPHICS_ASSERT(file.is_open(), LOG_CATEGORY, result::UNABLE_TO_OPEN_FILE, "File: '%s'!", CommonUtilities::ANSI(aPath));

	size_t bufferSize = 1048576;	//1 MB
	size_t cursor = 0;
	char* buffer = new char[bufferSize];

	do
	{
		if (cursor == bufferSize)
		{
			bufferSize = bufferSize << 1;
			char* newBuffer = new char[bufferSize];
			memcpy(newBuffer, buffer, cursor);
			delete[] buffer;
			buffer = newBuffer;
		}

		file.read(&buffer[cursor], bufferSize - cursor);

		cursor += file.gcount();

	} while (file.eof() == false);

	file.close();

	*outBuffer = buffer;
	*outSize = cursor;

	return result::OK;
#undef LOG_CATEGORY
}

graphics::result::Code graphics::CreateTexture2DFromFile(Device* aDevice, const char* aName, const wchar_t* aFilePath, Texture2D** outTexture2D, GRAPHICS_BIND_FLAGS aBindFlags, GRAPHICS_RESOURCE_FLAGS aResourceFlags, GRAPHICS_USAGE aUsage, GRAPHICS_CPU_ACCESS_FLAGS aCPUAccessFlags)
{
#define LOG_CATEGORY CLASS_CATEGORY"::CreateTexture2DFromFile"
	GRAPHICS_ASSERT(aDevice != nullptr, LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(aDevice) " cannot be nullptr!");
	GRAPHICS_ASSERT(outTexture2D != nullptr, LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(outTexture2D) " cannot be nullptr!");

	char* fileData;
	size_t fileSize;
	result::Code result = detail::ReadFile(aFilePath, &fileData, &fileSize);
	GRAPHICS_ASSERT_RESULT(result, LOG_CATEGORY, "Failed to read file: '%s'!", CommonUtilities::ANSI(aFilePath));

	result = CreateTexture2DFromDDS(
		aDevice,
		aName,
		fileData,
		fileSize,
		outTexture2D,
		aBindFlags,
		aResourceFlags,
		aUsage,
		aCPUAccessFlags
	);

	delete[] fileData;

	GRAPHICS_ASSERT_RESULT(result, LOG_CATEGORY, "Failed to create Texture2D from file!");

	return result::OK;
#undef LOG_CATEGORY
}

graphics::result::Code graphics::CreateTexture2DFromDDS(
	Device* aDevice,
	const char* aName,
	const void* aData,
	size_t aDataSize,
	Texture2D** outTexture2D,
	GRAPHICS_BIND_FLAGS aBindFlags,
	GRAPHICS_RESOURCE_FLAGS aResourceFlags,
	GRAPHICS_USAGE aUsage,
	GRAPHICS_CPU_ACCESS_FLAGS aCPUAccessFlags
)
{
#define LOG_CATEGORY CLASS_CATEGORY"::CreateTexture2DFromDDS"
	GRAPHICS_ASSERT(aDevice != nullptr, LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(aDevice) " cannot be nullptr!");
	GRAPHICS_ASSERT(outTexture2D != nullptr, LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(outTexture2D) " cannot be nullptr!");

	Device_DX11Impl* device = dynamic_cast<Device_DX11Impl*>(aDevice);
	GRAPHICS_ASSERT(device != nullptr, LOG_CATEGORY, result::INVALID_IMPLEMENTATION, STRINGIFY(aDevice) " could not be cast to '" STRINGIFY(Device_DX11Impl*) "'!");

	Microsoft::WRL::ComPtr<ID3D11Resource> dxResource;
	HRESULT result = DirectX::CreateDDSTextureFromMemoryEx(
		device->GetDevice(),
		reinterpret_cast<const uint8_t*>(aData),
		aDataSize,
		0,
		dx11::GetD3D11Usage(aUsage),
		dx11::GetD3D11BindFlags(aBindFlags),
		dx11::GetD3D11CPUAccessFlag(aCPUAccessFlags),
		dx11::GetD3D11ResourceMiscFlag(aResourceFlags),
		DirectX::DDS_LOADER_DEFAULT,
		&dxResource,
		NULL
	);
	GRAPHICS_ASSERT_WIN32_API_CALL(result, DirectX::CreateDDSTextureFromMemory, LOG_CATEGORY);

	Microsoft::WRL::ComPtr<ID3D11Texture2D> dxTexture;
	result = dxResource.As(&dxTexture);
	GRAPHICS_ASSERT_WIN32_API_CALL(result, Microsoft::WRL::ComPtr<ID3D11Resource>::As, LOG_CATEGORY);

	D3D11_TEXTURE2D_DESC dx11Desc = {};
	dxTexture->GetDesc(&dx11Desc);

	Texture2DDesc desc = {};
	desc.BindFlags = dx11::GetGraphicsTextureFlags(static_cast<D3D11_BIND_FLAG>(dx11Desc.BindFlags));
	desc.Name = aName;
	desc.Width = dx11Desc.Width;
	desc.Height = dx11Desc.Height;
	desc.MipLevels = dx11Desc.MipLevels;
	desc.NumTextures = dx11Desc.ArraySize;
	desc.Data = nullptr;
	desc.NumData = 0u;
	desc.Format = dx11::GetGraphicsFormat(dx11Desc.Format);
	desc.Usage = dx11::GetGraphicsUsage(dx11Desc.Usage);
	desc.CPUAccessFlags = dx11::GetGraphicsCPUAccessFlags(static_cast<D3D11_CPU_ACCESS_FLAG>(dx11Desc.CPUAccessFlags));

	*outTexture2D = new Texture2D_DX11Impl(device->CreateGraphicsID(), dxTexture.Get(), desc);
	(**outTexture2D).AddReference();

	(**outTexture2D).SetName(aName);

	return result::OK;
#undef LOG_CATEGORY
}

#undef FILE_CATEGORY
#undef CLASS_CATEGORY
