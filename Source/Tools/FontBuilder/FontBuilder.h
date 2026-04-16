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

	bool BuildSimpleAtlas(const std::filesystem::path& aSource, const std::filesystem::path& aTarget, unsigned int aFontSize, const wchar_t* aCharacters, size_t aNumCharacters);
	bool BuildAtlas(const std::filesystem::path& aSource, const std::filesystem::path& aTarget, unsigned int aFontSize, const wchar_t* aCharacters, size_t aNumCharacters);
private:
	Microsoft::WRL::ComPtr<ID3D11Device> myDevice;
	Microsoft::WRL::ComPtr<IDXGISwapChain> mySwapChain;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> myDeviceContext;
	Microsoft::WRL::ComPtr<ID2D1Factory> myFactory;
	Microsoft::WRL::ComPtr<IDWriteFactory> myWriteFactory;

	Microsoft::WRL::ComPtr<ID3D11Buffer> myJFAVertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> myJFAConstantBuffer;

	Microsoft::WRL::ComPtr<ID3D11InputLayout> myJFAInputLayout;
	Microsoft::WRL::ComPtr<ID3D11SamplerState> myJFAPointClampSamplerState;
	Microsoft::WRL::ComPtr<ID3D11SamplerState> myJFATrilinearClampSamplerState;

	Microsoft::WRL::ComPtr<ID3D11Texture2D> myJFATargetTexture;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> myJFARTV;

	Microsoft::WRL::ComPtr<ID3D11Texture2D> myJFAResourceTexture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> myJFASRV;

	Microsoft::WRL::ComPtr<ID3D11VertexShader> myJFAVertexShader;
	Microsoft::WRL::ComPtr<ID3D11GeometryShader> myJFAGeometryShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> myJFAPixelShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> myJFAInitPixelShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> myJFADownSamplePixelShader;
};