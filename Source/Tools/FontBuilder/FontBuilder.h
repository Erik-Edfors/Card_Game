#pragma once
#include <filesystem>

#include <d3d11_3.h>
#include <d2d1_3.h>
#include <dwrite_3.h>
#include <wrl.h>

class GraphicsEngine;

class FontBuilder
{
public:
	FontBuilder();
	~FontBuilder();

	bool Initialize();

	bool Build(const std::filesystem::path& aSource, const std::filesystem::path& aTarget, unsigned int aFontSize, const wchar_t* aCharacters, size_t aNumCharacters);
private:
	Microsoft::WRL::ComPtr<ID3D11Device> myDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> myDeviceContext;
	Microsoft::WRL::ComPtr<ID2D1Factory> myFactory;
	Microsoft::WRL::ComPtr<IDWriteFactory> myWriteFactory;
};