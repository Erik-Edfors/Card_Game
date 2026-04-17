#include "FontBuilder.h"

#include <Core/CommonUtilities/Serialization/StreamWrapper.hpp>
#include <Core/CommonUtilities/Serialization/Serializer.hpp>
#include <Core/Assets/Font.h>

#include <fstream>
#include <iostream>

struct JFAVertex
{
	float Rectangle[4];
	float UVRectangle[4];
};

struct JFABuffer
{
	float StepDistance[2];

	unsigned int _padding[2];
};

static void DebugExportTexture(ID3D11Device* aDevice, ID3D11DeviceContext* aContext, ID3D11Texture2D* aTexture, const wchar_t* aFile)
{
	D3D11_TEXTURE2D_DESC desc = {};
	aTexture->GetDesc(&desc);

	desc.Usage = D3D11_USAGE_STAGING;
	desc.BindFlags = NULL;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;

	Microsoft::WRL::ComPtr<ID3D11Texture2D> texture;
	aDevice->CreateTexture2D(&desc, NULL, &texture);

	aContext->CopyResource(texture.Get(), aTexture);

	D3D11_MAPPED_SUBRESOURCE mapped = {};
	aContext->Map(texture.Get(), 0, D3D11_MAP_READ, NULL, &mapped);

	const unsigned int imageSize = desc.Height * ((desc.Width * 3 + 3) & 0xfffffffc);
	constexpr unsigned int bmpStride = 3;

	std::unique_ptr<char[]> imageData = std::make_unique<char[]>(imageSize);

	for (unsigned int y = 0; y < desc.Height; y++)
	{
		for (unsigned int x = 0; x < desc.Width; x++)
		{
			const unsigned int bmpIndex = y * ((desc.Width * 3 + 3) & 0xfffffffc) + x * bmpStride;

			switch (desc.Format)
			{
				case DXGI_FORMAT_R32G32B32A32_FLOAT:
				{
					constexpr unsigned int stride = 16;
					const unsigned int resourceIndex = y * mapped.RowPitch + x * stride;
					imageData[bmpIndex + 0] = static_cast<char>(*(reinterpret_cast<float*>(static_cast<char*>(mapped.pData) + resourceIndex + 0 * 4)) * 255.f);
					imageData[bmpIndex + 1] = static_cast<char>(*(reinterpret_cast<float*>(static_cast<char*>(mapped.pData) + resourceIndex + 1 * 4)) * 255.f);
					imageData[bmpIndex + 2] = static_cast<char>(*(reinterpret_cast<float*>(static_cast<char*>(mapped.pData) + resourceIndex + 2 * 4)) * 255.f);
				}
				break;
				case DXGI_FORMAT_B8G8R8A8_UNORM:
				{
					constexpr unsigned int stride = 4;
					const unsigned int resourceIndex = y * mapped.RowPitch + x * stride;
					imageData[bmpIndex + 0] = *(reinterpret_cast<char*>(mapped.pData) + resourceIndex + 3);
					imageData[bmpIndex + 1] = *(reinterpret_cast<char*>(mapped.pData) + resourceIndex + 2);
					imageData[bmpIndex + 2] = *(reinterpret_cast<char*>(mapped.pData) + resourceIndex + 1);
				}
				break;
			}
		}
	}

	aContext->Unmap(texture.Get(), 0);

	const float pixelsPerMeter = 96.f * (1.f / 0.0254f);

	BITMAPINFOHEADER bmpHeader = {
		.biSize = sizeof(bmpHeader),
		.biWidth = static_cast<LONG>(desc.Width),
		.biHeight = -static_cast<LONG>(desc.Height),
		.biPlanes = 1,
		.biBitCount = 24,
		.biCompression = BI_RGB,
		.biSizeImage = imageSize,
		.biXPelsPerMeter = static_cast<LONG>(pixelsPerMeter),
		.biYPelsPerMeter = static_cast<LONG>(pixelsPerMeter),
		.biClrUsed = 0,
		.biClrImportant = 0
	};

	BITMAPFILEHEADER bmpFileHeader = {
		.bfType = 0x4d42,
		.bfSize = sizeof(bmpFileHeader) + sizeof(bmpHeader) + bmpStride * desc.Width * desc.Height,
		.bfReserved1 = 0,
		.bfReserved2 = 0,
		.bfOffBits = sizeof(bmpFileHeader) + sizeof(bmpHeader)
	};

	std::ofstream file(aFile, std::ios::binary);

	file.write(reinterpret_cast<char*>(&bmpFileHeader), sizeof(bmpFileHeader));
	file.write(reinterpret_cast<char*>(&bmpHeader), sizeof(bmpHeader));
	file.write(imageData.get(), imageSize);

	file.close();
}

static void ReleaseBuffer(void* aBuffer)
{
	delete[] reinterpret_cast<char*>(aBuffer);
}

static size_t LoadStreamIntoMemory(void** outBuffer, std::istream& aStream, size_t aMaxAllocation)
{
	if (aStream.eof() || aStream.fail())
	{
		return 0;
	}

	size_t bufferSize = 4096;
	size_t cursor = 0;
	char* buffer = new char[bufferSize];

	do
	{
		aStream.read(buffer + cursor, bufferSize - cursor);
		cursor += aStream.gcount();

		if (cursor == bufferSize)
		{
			size_t stagingSize = (std::min)(bufferSize * 2, aMaxAllocation);
			if (stagingSize == bufferSize)
			{
				ReleaseBuffer(buffer);
				return 0;
			}

			char* staging = new char[stagingSize];

			errno_t error = memcpy_s(staging, stagingSize, buffer, bufferSize);

			ReleaseBuffer(buffer);

			if (error != 0)
			{
				ReleaseBuffer(staging);
				return 0;
			}

			buffer = staging;
			bufferSize = stagingSize;
		}
	} while (not aStream.eof() && not aStream.fail());

	*outBuffer = buffer;
	return cursor;
}

static HRESULT UpdateBuffer(ID3D11DeviceContext* aContext, ID3D11Buffer* aBuffer, const void* aData, size_t aDataSize)
{
	D3D11_SUBRESOURCE_DATA data = {
		.pSysMem = aData,
		.SysMemPitch = 0u,
		.SysMemSlicePitch = 0u
	};

	D3D11_MAPPED_SUBRESOURCE mappedVertexBuffer = {};
	HRESULT result = aContext->Map(aBuffer, 0, D3D11_MAP_WRITE_DISCARD, NULL, &mappedVertexBuffer);
	if (FAILED(result))
	{
		//TODO: Log.
		return result;
	}
	errno_t error = memcpy_s(mappedVertexBuffer.pData, mappedVertexBuffer.DepthPitch, aData, aDataSize);
	if (error != 0)
	{
		return E_BOUNDS;
	}
	aContext->Unmap(aBuffer, 0);

	return S_OK;
}

FontBuilder::FontBuilder()
{
}
FontBuilder::~FontBuilder()
{
}

bool FontBuilder::Initialize()
{
	constexpr unsigned int JFATextureWidth = 4096;
	constexpr unsigned int JFATextureHeight = 4096;

	//DXGI_SWAP_CHAIN_DESC desc = {
	//	.BufferDesc = DXGI_MODE_DESC{
	//		.Width = 32,
	//		.Height = 32,
	//		.RefreshRate = DXGI_RATIONAL{
	//			.Numerator = 1u,
	//			.Denominator = 60u
	//		},
	//		.Format = DXGI_FORMAT_R32G32B32A32_FLOAT,
	//		.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED,
	//		.Scaling = DXGI_MODE_SCALING_CENTERED
	//	},
	//	.SampleDesc = DXGI_SAMPLE_DESC{
	//		.Count = 1u,
	//		.Quality = 0u
	//	},
	//	.BufferUsage = DXGI_USAGE_BACK_BUFFER,
	//	.BufferCount = 2,
	//	.OutputWindow = 
	//};

	//DXGI_MODE_DESC BufferDesc;
	//DXGI_SAMPLE_DESC SampleDesc;
	//DXGI_USAGE BufferUsage;
	//UINT BufferCount;
	//HWND OutputWindow;
	//BOOL Windowed;
	//DXGI_SWAP_EFFECT SwapEffect;
	//UINT Flags;

	HRESULT result = D3D11CreateDevice(
		NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		D3D11_CREATE_DEVICE_BGRA_SUPPORT,
		NULL,
		0,
		D3D11_SDK_VERSION,
		&myDevice,
		NULL,
		&myDeviceContext
	);
	if (FAILED(result))
	{
		//TODO: Log
		return false;
	}

	result = D2D1CreateFactory<ID2D1Factory>(D2D1_FACTORY_TYPE_SINGLE_THREADED, &myFactory);
	if (FAILED(result))
	{
		//TODO: Log
		return false;
	}

	result = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), &myWriteFactory);
	if (FAILED(result))
	{
		//TODO: Log
		return false;
	}


	{
		std::ifstream file("JFA_VS.cso", std::ios::binary);
		void* byteCode = nullptr;
		size_t byteCodeSize = LoadStreamIntoMemory(&byteCode, file, 1'048'576);

		result = myDevice->CreateVertexShader(byteCode, byteCodeSize, NULL, &myJFAVertexShader);
		if (FAILED(result))
		{
			ReleaseBuffer(byteCode);
			//TODO: Log
			return false;
		}

		const D3D11_INPUT_ELEMENT_DESC elements[2] = {
			D3D11_INPUT_ELEMENT_DESC{
				.SemanticName = "RECTANGLE",
				.SemanticIndex = 0,
				.Format = DXGI_FORMAT_R32G32B32A32_FLOAT,
				.InputSlot = 0u,
				.AlignedByteOffset = 0xffffffff,
				.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA,
				.InstanceDataStepRate = 0u
			},
			D3D11_INPUT_ELEMENT_DESC{
				.SemanticName = "UVRECTANGLE",
				.SemanticIndex = 0,
				.Format = DXGI_FORMAT_R32G32B32A32_FLOAT,
				.InputSlot = 0u,
				.AlignedByteOffset = 0xffffffff,
				.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA,
				.InstanceDataStepRate = 0u
			}
		};

		result = myDevice->CreateInputLayout(elements, 2, byteCode, byteCodeSize, &myJFAInputLayout);
		if (FAILED(result))
		{
			ReleaseBuffer(byteCode);
			//TODO: Log
			return false;
		}

		ReleaseBuffer(byteCode);
	}

	{
		std::ifstream file("JFA_GS.cso", std::ios::binary);
		void* byteCode = nullptr;
		size_t byteCodeSize = LoadStreamIntoMemory(&byteCode, file, 1'048'576);

		result = myDevice->CreateGeometryShader(byteCode, byteCodeSize, NULL, &myJFAGeometryShader);

		ReleaseBuffer(byteCode);
		if (FAILED(result))
		{
			//TODO: Log
			return false;
		}
	}

	{
		std::ifstream file("JFA_PS.cso", std::ios::binary);
		void* byteCode = nullptr;
		size_t byteCodeSize = LoadStreamIntoMemory(&byteCode, file, 1'048'576);

		result = myDevice->CreatePixelShader(byteCode, byteCodeSize, NULL, &myJFAPixelShader);

		ReleaseBuffer(byteCode);
		if (FAILED(result))
		{
			//TODO: Log
			return false;
		}
	}

	{
		std::ifstream file("JFA_Init_PS.cso", std::ios::binary);
		void* byteCode = nullptr;
		size_t byteCodeSize = LoadStreamIntoMemory(&byteCode, file, 1'048'576);

		result = myDevice->CreatePixelShader(byteCode, byteCodeSize, NULL, &myJFAInitPixelShader);

		ReleaseBuffer(byteCode);
		if (FAILED(result))
		{
			//TODO: Log
			return false;
		}
	}

	{
		std::ifstream file("JFA_DownSample_PS.cso", std::ios::binary);
		void* byteCode = nullptr;
		size_t byteCodeSize = LoadStreamIntoMemory(&byteCode, file, 1'048'576);

		result = myDevice->CreatePixelShader(byteCode, byteCodeSize, NULL, &myJFADownSamplePixelShader);

		ReleaseBuffer(byteCode);
		if (FAILED(result))
		{
			//TODO: Log
			return false;
		}
	}

	{
		std::ifstream file("JFA_ToSID_PS.cso", std::ios::binary);
		void* byteCode = nullptr;
		size_t byteCodeSize = LoadStreamIntoMemory(&byteCode, file, 1'048'576);

		result = myDevice->CreatePixelShader(byteCode, byteCodeSize, NULL, &myJFAToSIDPixelShader);

		ReleaseBuffer(byteCode);
		if (FAILED(result))
		{
			//TODO: Log
			return false;
		}
	}

	{
		D3D11_SAMPLER_DESC desc = {
			.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT,
			.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP,
			.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP,
			.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP,
			.MipLODBias = 0,
			.MaxAnisotropy = 0,
			.ComparisonFunc = D3D11_COMPARISON_NEVER,
			.BorderColor = { 0.f, 0.f, 0.f, 0.f },
			.MinLOD = 0.f,
			.MaxLOD = D3D11_FLOAT32_MAX
		};

		result = myDevice->CreateSamplerState(&desc, &myJFAPointClampSamplerState);
		if (FAILED(result))
		{
			//TODO: Log
			return false;
		}

		desc.Filter = D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR;
		result = myDevice->CreateSamplerState(&desc, &myJFATrilinearClampSamplerState);
		if (FAILED(result))
		{
			//TODO: Log
			return false;
		}
	}

	{
		D3D11_TEXTURE2D_DESC desc = {
			.Width = JFATextureWidth,
			.Height = JFATextureHeight,
			.MipLevels = 1u,
			.ArraySize = 1u,
			.Format = DXGI_FORMAT_R32G32B32A32_FLOAT,
			.SampleDesc = DXGI_SAMPLE_DESC{
				.Count = 1u,
				.Quality = 0
			},
			.Usage = D3D11_USAGE_DEFAULT,
			.BindFlags = D3D11_BIND_RENDER_TARGET,
			.CPUAccessFlags = NULL,
			.MiscFlags = NULL
		};

		result = myDevice->CreateTexture2D(&desc, NULL, &myJFATargetTexture);
		if (FAILED(result))
		{
			//TODO: Log
			return false;
		}

		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

		result = myDevice->CreateTexture2D(&desc, NULL, &myJFAResourceTexture);
		if (FAILED(result))
		{
			//TODO: Log
			return false;
		}

		D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {
			.Format = DXGI_FORMAT_UNKNOWN,
			.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D,
			.Texture2D = D3D11_TEX2D_RTV{
				.MipSlice = 0
			}
		};

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {
			.Format = DXGI_FORMAT_UNKNOWN,
			.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D,
			.Texture2D = D3D11_TEX2D_SRV{
				.MostDetailedMip = 0u,
				.MipLevels = 1u
			}
		};

		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
		unsigned int width = JFATextureWidth / 2;
		unsigned int height = JFATextureWidth / 2;
		while (width > 1 && height > 1)
		{
			__DownsampleObjects& sampleObjects = myDownSampleChain.emplace_back();
			desc.Width = width;
			desc.Height = height;

			result = myDevice->CreateTexture2D(&desc, NULL, &sampleObjects.Texture);
			if (FAILED(result))
			{
				//TODO: Log
				return false;
			}

			result = myDevice->CreateShaderResourceView(sampleObjects.Texture.Get(), &srvDesc, &sampleObjects.SRV);
			if (FAILED(result))
			{
				//TODO: Log
				return false;
			}

			result = myDevice->CreateRenderTargetView(sampleObjects.Texture.Get(), &rtvDesc, &sampleObjects.RTV);
			if (FAILED(result))
			{
				//TODO: Log
				return false;
			}

			width /= 2;
			height /= 2;
		}
	}

	{
		D3D11_RENDER_TARGET_VIEW_DESC desc = {
			.Format = DXGI_FORMAT_UNKNOWN,
			.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D,
			.Texture2D = D3D11_TEX2D_RTV{
				.MipSlice = 0
			}
		};

		result = myDevice->CreateRenderTargetView(myJFATargetTexture.Get(), &desc, &myJFARTV);
		if (FAILED(result))
		{
			//TODO: Log
			return false;
		}
	}

	{
		D3D11_SHADER_RESOURCE_VIEW_DESC desc = {
			.Format = DXGI_FORMAT_UNKNOWN,
			.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D,
			.Texture2D = D3D11_TEX2D_SRV{
				.MostDetailedMip = 0u,
				.MipLevels = 1u
			}
		};

		result = myDevice->CreateShaderResourceView(myJFAResourceTexture.Get(), &desc, &myJFASRV);
		if (FAILED(result))
		{
			//TODO: Log
			return false;
		}
	}

	{
		D3D11_BUFFER_DESC desc = {
			.ByteWidth = sizeof(JFAVertex),
			.Usage = D3D11_USAGE_DYNAMIC,
			.BindFlags = D3D11_BIND_VERTEX_BUFFER,
			.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE,
			.MiscFlags = NULL,
			.StructureByteStride = sizeof(JFAVertex)
		};

		result = myDevice->CreateBuffer(&desc, NULL, &myJFAVertexBuffer);
		if (FAILED(result))
		{
			//TODO: Log
			return false;
		}
	}

	{
		D3D11_BUFFER_DESC desc = {
			.ByteWidth = sizeof(JFABuffer),
			.Usage = D3D11_USAGE_DYNAMIC,
			.BindFlags = D3D11_BIND_CONSTANT_BUFFER,
			.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE,
			.MiscFlags = NULL,
			.StructureByteStride = NULL
		};

		result = myDevice->CreateBuffer(&desc, NULL, &myJFAConstantBuffer);
		if (FAILED(result))
		{
			//TODO: Log
			return false;
		}
	}

	return true;
}

bool FontBuilder::BuildSimpleAtlas(const std::filesystem::path& aSource, const std::filesystem::path& aTarget, unsigned int aFontSize, const wchar_t* aCharacters, size_t aNumCharacters)
{
	const std::filesystem::path sourceAbsolute = (std::filesystem::current_path() / aSource).lexically_normal();


	assets::Font output = {};
	output.Glyphs.resize(aNumCharacters);
	output.FontSize = static_cast<unsigned short>(aFontSize);


	Microsoft::WRL::ComPtr<IDWriteFontFile> fontFile;
	HRESULT result = myWriteFactory->CreateFontFileReference(
		sourceAbsolute.c_str(),
		//L"C:\\Windows\\Fonts\\arial.ttf",
		NULL,
		&fontFile
	);
	if (FAILED(result))
	{
		//TODO: Log
		return false;
	}

	BOOL isSupported = FALSE;
	DWRITE_FONT_FILE_TYPE fontType = DWRITE_FONT_FILE_TYPE_UNKNOWN;
	DWRITE_FONT_FACE_TYPE fontFaceType = DWRITE_FONT_FACE_TYPE_UNKNOWN;
	UINT32 numFaces = 0;
	result = fontFile->Analyze(&isSupported, &fontType, &fontFaceType, &numFaces);
	if (FAILED(result))
	{
		//TODO: Log
		return false;
	}
	if (not isSupported)
	{
		//TODO: Log
		return false;
	}

	Microsoft::WRL::ComPtr<IDWriteFontFace> fontFace;
	result = myWriteFactory->CreateFontFace(
		fontFaceType,
		1,
		fontFile.GetAddressOf(),
		0,
		DWRITE_FONT_SIMULATIONS_NONE,
		&fontFace
	);
	if (FAILED(result))
	{
		//TODO: Log
		return false;
	}

	DWRITE_FONT_METRICS fontMetrics;
	fontFace->GetMetrics(&fontMetrics);

	constexpr float dpi = 96.f;
	const float DUToEm = 1.f / static_cast<float>(fontMetrics.designUnitsPerEm);
	const float EmToPx = static_cast<float>(aFontSize);
	//const float PxToEm = 1.f / EmToPx;

	output.AdvanceHeight = static_cast<float>(fontMetrics.ascent + fontMetrics.descent + fontMetrics.lineGap) * DUToEm * EmToPx;
	output.StrikethroughHeight = static_cast<float>(fontMetrics.strikethroughPosition) * DUToEm * EmToPx;
	output.StrikethroughThickness = static_cast<float>(fontMetrics.strikethroughThickness) * DUToEm * EmToPx;
	output.UnderlineHeight = static_cast<float>(fontMetrics.underlinePosition) * DUToEm * EmToPx;
	output.UnderlineThickness = static_cast<float>(fontMetrics.underlineThickness) * DUToEm * EmToPx;


	std::unique_ptr<UINT32[]> codePoints = std::make_unique<UINT32[]>(aNumCharacters);
	std::unique_ptr<UINT16[]> glyphIndices = std::make_unique<UINT16[]>(aNumCharacters);
	std::unique_ptr<DWRITE_GLYPH_METRICS[]> glyphMetrics = std::make_unique<DWRITE_GLYPH_METRICS[]>(aNumCharacters);

	for (size_t i = 0; i < aNumCharacters; i++)
	{
		codePoints[i] = aCharacters[i];
		output.Glyphs[i].Character = aCharacters[i];
	}

	result = fontFace->GetGlyphIndicesW(codePoints.get(), static_cast<UINT32>(aNumCharacters), glyphIndices.get());
	if (FAILED(result))
	{
		//TODO: Log
		return false;
	}

	result = fontFace->GetDesignGlyphMetrics(glyphIndices.get(), static_cast<UINT32>(aNumCharacters), glyphMetrics.get(), FALSE);
	if (FAILED(result))
	{
		//TODO: Log
		return false;
	}

	unsigned int glyphWidth = 0;
	unsigned int glyphHeight = 0;
	int xOffset = 0;
	int yOffset = 0;
	for (size_t i = 0; i < aNumCharacters; i++)
	{
		const DWRITE_GLYPH_METRICS& metrics = glyphMetrics[i];

		int blackboxLeft = metrics.leftSideBearing;
		int blackboxRight = metrics.advanceWidth - metrics.rightSideBearing;

		int blackboxTop = -(glyphMetrics[i].verticalOriginY - glyphMetrics[i].topSideBearing);
		int blackboxBottom = glyphMetrics[i].advanceHeight - glyphMetrics[i].verticalOriginY - glyphMetrics[i].bottomSideBearing;

		int width = blackboxRight - blackboxLeft;
		int height = blackboxBottom - blackboxTop;

		output.Glyphs[i].Advance = static_cast<float>(metrics.advanceWidth) * DUToEm * EmToPx;
		output.Glyphs[i].Bounds[0] = static_cast<float>(blackboxLeft) * DUToEm * EmToPx;
		output.Glyphs[i].Bounds[1] = static_cast<float>(blackboxTop) * DUToEm * EmToPx - 1.f;	// -1.f offset because it was clipping the top for some reason.
		output.Glyphs[i].Bounds[2] = static_cast<float>(blackboxRight) * DUToEm * EmToPx;
		output.Glyphs[i].Bounds[3] = static_cast<float>(blackboxBottom) * DUToEm * EmToPx;

		if (width > 0 && static_cast<unsigned int>(width) > glyphWidth)
		{
			glyphWidth = static_cast<unsigned int>(width);
		}
		if (height > 0 && static_cast<unsigned int>(height) > glyphHeight)
		{
			glyphHeight = static_cast<unsigned int>(height);
		}

		if (-blackboxLeft > xOffset)
		{
			xOffset = -blackboxLeft;
		}
		if (-blackboxTop > yOffset)
		{
			yOffset = -blackboxTop;
		}
	}

	constexpr float cellBuffer = 8.f;

	const float cellWidth = glyphWidth * DUToEm * EmToPx + cellBuffer;
	const float cellHeight = glyphHeight * DUToEm * EmToPx + cellBuffer;
	const float glyphCellXOffset = xOffset * DUToEm * EmToPx + 0.5f * cellBuffer;
	const float glyphCellYOffset = yOffset * DUToEm * EmToPx + 0.5f * cellBuffer;

	unsigned int textureWidth = 32;

	while ((textureWidth / cellWidth) * (textureWidth / cellHeight) < static_cast<unsigned int>(aNumCharacters))
	{
		textureWidth *= 2;

		if (textureWidth > D3D11_REQ_TEXTURE2D_U_OR_V_DIMENSION)
		{
			//TODO: Log
			return false;
		}
	}

	unsigned int textureHeight = textureWidth;

	while ((textureWidth / cellWidth) * ((textureHeight / 2) / cellHeight) >= static_cast<unsigned int>(aNumCharacters))
	{
		textureHeight /= 2;
	}

	const unsigned int xCellCount = static_cast<unsigned int>(static_cast<float>(textureWidth) / cellWidth);
	//const unsigned int yCellCount = static_cast<unsigned int>(static_cast<float>(textureHeight) / cellHeight);

	Microsoft::WRL::ComPtr<ID3D11Texture2D> stagingTexture;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> texture;
	{
		D3D11_TEXTURE2D_DESC desc = {
			.Width = textureWidth,
			.Height = textureHeight,
			.MipLevels = 1,
			.ArraySize = 1,
			.Format = DXGI_FORMAT_B8G8R8A8_UNORM,
			.SampleDesc = {.Count = 1, .Quality = 0 },
			.Usage = D3D11_USAGE_DEFAULT,
			.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE,
			.CPUAccessFlags = NULL,
			.MiscFlags = NULL
		};

		result = myDevice->CreateTexture2D(&desc, NULL, &texture);
		if (FAILED(result))
		{
			//TODO: Log
			return false;
		}

		desc.Usage = D3D11_USAGE_STAGING;
		desc.BindFlags = NULL;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;

		result = myDevice->CreateTexture2D(&desc, NULL, &stagingTexture);
		if (FAILED(result))
		{
			//TODO: Log
			return false;
		}
	}

	output.Atlas.Width = static_cast<unsigned short>(textureWidth);
	output.Atlas.Height = static_cast<unsigned short>(textureHeight);

	Microsoft::WRL::ComPtr<IDXGISurface> surface;
	result = texture.As(&surface);
	if (FAILED(result))
	{
		//TODO: Log
		return false;
	}

	D2D1_RENDER_TARGET_PROPERTIES properties = {
		.type = D2D1_RENDER_TARGET_TYPE_DEFAULT,
		.pixelFormat = {
			.format = DXGI_FORMAT_B8G8R8A8_UNORM,
			.alphaMode = D2D1_ALPHA_MODE_PREMULTIPLIED
		},
		.dpiX = dpi,
		.dpiY = dpi,
		.usage = D2D1_RENDER_TARGET_USAGE_NONE,
		.minLevel = D2D1_FEATURE_LEVEL_DEFAULT
	};

	Microsoft::WRL::ComPtr<ID2D1RenderTarget> renderTarget;
	result = myFactory->CreateDxgiSurfaceRenderTarget(
		surface.Get(),
		&properties,
		&renderTarget
	);
	if (FAILED(result))
	{
		//TODO: Log
		return false;
	}

	D2D1_MATRIX_3X2_F transform = {};

	transform.m11 = 1.f;
	transform.m12 = 0.f;
	transform.m21 = 0.f;
	transform.m22 = 1.f;
	transform.dx = 0.f;
	transform.dy = 0.f;

	renderTarget->SetTransform(transform);

	Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> brush;
	result = renderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White, 1.f), &brush);
	if (FAILED(result))
	{
		//TODO: Log
		return false;
	}

	renderTarget->BeginDraw();
	for (size_t i = 0; i < aNumCharacters; i++)
	{

		const FLOAT glyphAdvance = static_cast<FLOAT>(glyphMetrics[i].advanceWidth) * DUToEm;
		const DWRITE_GLYPH_OFFSET glyphOffset = {
			.advanceOffset = 0.f,
			.ascenderOffset = 0.f
		};

		const float originX = static_cast<float>(i % xCellCount) * cellWidth + glyphCellXOffset;
		const float originY = static_cast<float>(i / xCellCount) * cellHeight + glyphCellYOffset;

		output.Glyphs[i].AtlasRect[0] = (output.Glyphs[i].Bounds[0] + originX) / static_cast<float>(textureWidth);
		output.Glyphs[i].AtlasRect[1] = (output.Glyphs[i].Bounds[1] + originY) / static_cast<float>(textureHeight);
		output.Glyphs[i].AtlasRect[2] = (output.Glyphs[i].Bounds[2] + originX) / static_cast<float>(textureWidth);
		output.Glyphs[i].AtlasRect[3] = (output.Glyphs[i].Bounds[3] + originY) / static_cast<float>(textureHeight);

		const DWRITE_GLYPH_RUN glyphRun = {
			.fontFace = fontFace.Get(),
			.fontEmSize = static_cast<FLOAT>(aFontSize),
			.glyphCount = 1,
			.glyphIndices = &glyphIndices[i],
			.glyphAdvances = &glyphAdvance,
			.glyphOffsets = &glyphOffset,
			.isSideways = FALSE,
			.bidiLevel = 0
		};

		renderTarget->DrawGlyphRun(
			{ originX, originY },
			&glyphRun,
			brush.Get(),
			DWRITE_MEASURING_MODE_NATURAL
		);
	}
	result = renderTarget->EndDraw();
	if (FAILED(result))
	{
		//TODO: Log
		return false;
	}


	myDeviceContext->CopyResource(stagingTexture.Get(), texture.Get());

	D3D11_MAPPED_SUBRESOURCE mappedResource = {};
	result = myDeviceContext->Map(stagingTexture.Get(), 0, D3D11_MAP_READ, NULL, &mappedResource);
	if (FAILED(result))
	{
		//TODO: Log
		return false;
	}

	output.Atlas.RowPitch = mappedResource.RowPitch;
	output.Atlas.DepthPitch = mappedResource.DepthPitch;
	output.Atlas.Data.resize(mappedResource.DepthPitch);
	errno_t error = memcpy_s(output.Atlas.Data.data(), output.Atlas.Data.size() * sizeof(output.Atlas.Data[0]), mappedResource.pData, mappedResource.DepthPitch);

	myDeviceContext->Unmap(stagingTexture.Get(), 0);

	if (error != 0)
	{
		//TODO: Log
		return false;
	}

	std::ofstream file(aTarget, std::ios::binary);

	CommonUtilities::StreamWriter writer(file);
	CommonUtilities::Serializer serializer;
	if (not serializer.Write(assets::FONT_FILE_HEADER, writer))
	{
		//TODO: Log
		return false;
	}
	if (not serializer.Write(output, writer))
	{
		//TODO: Log
		return false;
	}

	return true;

	//D3D11_MAPPED_SUBRESOURCE mappedResource = {};
	//result = myDeviceContext->Map(stagingTexture.Get(), 0, D3D11_MAP_READ, NULL, &mappedResource);

	//const unsigned int imageSize = desc.Height * ((desc.Width * 3 + 3) & 0xfffffffc);
	//constexpr unsigned int stride = 4;
	//constexpr unsigned int bmpStride = 3;

	//std::unique_ptr<char[]> imageData = std::make_unique<char[]>(imageSize);

	//for (unsigned int y = 0; y < desc.Height; y++)
	//{
	//	for (unsigned int x = 0; x < desc.Width; x++)
	//	{
	//		const unsigned int resourceIndex = y * mappedResource.RowPitch + x * stride;
	//		const unsigned int bmpIndex = y * ((desc.Width * 3 + 3) & 0xfffffffc) + x * bmpStride;

	//		imageData[bmpIndex + 0] = reinterpret_cast<char*>(mappedResource.pData)[resourceIndex + 2];
	//		imageData[bmpIndex + 1] = reinterpret_cast<char*>(mappedResource.pData)[resourceIndex + 1];
	//		imageData[bmpIndex + 2] = reinterpret_cast<char*>(mappedResource.pData)[resourceIndex + 0];
	//	}
	//}

	//myDeviceContext->Unmap(stagingTexture.Get(), 0);

	//const float pixelsPerMeter = dpi * (1.f / 0.0254f);

	//BITMAPINFOHEADER bmpHeader = {
	//	.biSize = sizeof(bmpHeader),
	//	.biWidth = static_cast<LONG>(desc.Width),
	//	.biHeight = -static_cast<LONG>(desc.Height),
	//	.biPlanes = 1,
	//	.biBitCount = 24,
	//	.biCompression = BI_RGB,
	//	.biSizeImage = imageSize,
	//	.biXPelsPerMeter = static_cast<LONG>(pixelsPerMeter),
	//	.biYPelsPerMeter = static_cast<LONG>(pixelsPerMeter),
	//	.biClrUsed = 0,
	//	.biClrImportant = 0
	//};

	//BITMAPFILEHEADER bmpFileHeader = {
	//	.bfType = 0x4d42,
	//	.bfSize = sizeof(bmpFileHeader) + sizeof(bmpHeader) + bmpStride * desc.Width * desc.Height,
	//	.bfReserved1 = 0,
	//	.bfReserved2 = 0,
	//	.bfOffBits = sizeof(bmpFileHeader) + sizeof(bmpHeader)
	//};

	//std::ofstream file(aTarget);

	//file.write(reinterpret_cast<char*>(&bmpFileHeader), sizeof(bmpFileHeader));
	//file.write(reinterpret_cast<char*>(&bmpHeader), sizeof(bmpHeader));
	//file.write(imageData.get(), imageSize);
}

bool FontBuilder::BuildAtlas(const std::filesystem::path& aSource, const std::filesystem::path& aTarget, unsigned int aFontSize, const wchar_t* aCharacters, size_t aNumCharacters)
{
	assets::Font output;

	D3D11_TEXTURE2D_DESC jfaTargetDesc = {};
	myJFATargetTexture->GetDesc(&jfaTargetDesc);

	const unsigned int jfaWidth = jfaTargetDesc.Width;
	const unsigned int jfaHeight = jfaTargetDesc.Height;

#pragma region Loading Font Face

	const std::filesystem::path sourceAbsolute = (std::filesystem::current_path() / aSource).lexically_normal();

	Microsoft::WRL::ComPtr<IDWriteFontFile> fontFile;
	HRESULT result = myWriteFactory->CreateFontFileReference(
		sourceAbsolute.c_str(),
		NULL,
		&fontFile
	);
	if (FAILED(result))
	{
		//TODO: Log
		return false;
	}

	BOOL isSupported = FALSE;
	DWRITE_FONT_FILE_TYPE fontType = DWRITE_FONT_FILE_TYPE_UNKNOWN;
	DWRITE_FONT_FACE_TYPE fontFaceType = DWRITE_FONT_FACE_TYPE_UNKNOWN;
	UINT32 numFaces = 0;
	result = fontFile->Analyze(&isSupported, &fontType, &fontFaceType, &numFaces);
	if (FAILED(result))
	{
		//TODO: Log
		return false;
	}
	if (not isSupported)
	{
		//TODO: Log
		return false;
	}

	Microsoft::WRL::ComPtr<IDWriteFontFace> fontFace;
	result = myWriteFactory->CreateFontFace(
		fontFaceType,
		1,
		fontFile.GetAddressOf(),
		0,
		DWRITE_FONT_SIMULATIONS_NONE,
		&fontFace
	);
	if (FAILED(result))
	{
		//TODO: Log
		return false;
	}

#pragma endregion

#pragma region Load Font Metrics

	DWRITE_FONT_METRICS fontMetrics;
	fontFace->GetMetrics(&fontMetrics);

	constexpr float dpi = 96.f;
	//const float EmToDIP = 16.f;
	//const float DIPToPx = 96.f / dpi;
	const float DUToEm = 1.f / static_cast<float>(fontMetrics.designUnitsPerEm);
	//const float PxToEm = 1.f / EmToPx;

	output.FontSize = static_cast<unsigned short>(aFontSize);
	output.AdvanceHeight = static_cast<float>(fontMetrics.ascent + fontMetrics.descent + fontMetrics.lineGap) * DUToEm;
	output.StrikethroughHeight = static_cast<float>(fontMetrics.strikethroughPosition) * DUToEm;
	output.StrikethroughThickness = static_cast<float>(fontMetrics.strikethroughThickness) * DUToEm;
	output.UnderlineHeight = static_cast<float>(fontMetrics.underlinePosition) * DUToEm;
	output.UnderlineThickness = static_cast<float>(fontMetrics.underlineThickness) * DUToEm;

	std::unique_ptr<UINT32[]> codePoints = std::make_unique<UINT32[]>(aNumCharacters);
	std::unique_ptr<UINT16[]> glyphIndices = std::make_unique<UINT16[]>(aNumCharacters);
	std::unique_ptr<DWRITE_GLYPH_METRICS[]> glyphMetrics = std::make_unique<DWRITE_GLYPH_METRICS[]>(aNumCharacters);

	output.Glyphs.resize(aNumCharacters);
	for (size_t i = 0; i < aNumCharacters; i++)
	{
		codePoints[i] = aCharacters[i];
		output.Glyphs[i].Character = aCharacters[i];
	}

	result = fontFace->GetGlyphIndicesW(codePoints.get(), static_cast<UINT32>(aNumCharacters), glyphIndices.get());
	if (FAILED(result))
	{
		//TODO: Log
		return false;
	}

	result = fontFace->GetDesignGlyphMetrics(glyphIndices.get(), static_cast<UINT32>(aNumCharacters), glyphMetrics.get(), FALSE);
	if (FAILED(result))
	{
		//TODO: Log
		return false;
	}

	unsigned int glyphWidthDU = 0;
	unsigned int glyphHeightDU = 0;
	int xOffsetDU = 0;
	int yOffsetDU = 0;
	for (size_t i = 0; i < aNumCharacters; i++)
	{
		const DWRITE_GLYPH_METRICS& metrics = glyphMetrics[i];

		int blackboxLeftDU = metrics.leftSideBearing;
		int blackboxRightDU = metrics.advanceWidth - metrics.rightSideBearing;

		int blackboxTopDU = -(glyphMetrics[i].verticalOriginY - glyphMetrics[i].topSideBearing);
		int blackboxBottomDU = glyphMetrics[i].advanceHeight - glyphMetrics[i].verticalOriginY - glyphMetrics[i].bottomSideBearing;

		int widthDU = blackboxRightDU - blackboxLeftDU;
		int heightDU = blackboxBottomDU - blackboxTopDU;

		output.Glyphs[i].Advance = static_cast<float>(metrics.advanceWidth) * DUToEm;
		output.Glyphs[i].Bounds[0] = static_cast<float>(blackboxLeftDU) * DUToEm;
		output.Glyphs[i].Bounds[1] = static_cast<float>(blackboxTopDU) * DUToEm;	// Might need a small negative offset to prevent clipping.
		output.Glyphs[i].Bounds[2] = static_cast<float>(blackboxRightDU) * DUToEm;
		output.Glyphs[i].Bounds[3] = static_cast<float>(blackboxBottomDU) * DUToEm;

		if (widthDU > 0 && static_cast<unsigned int>(widthDU) > glyphWidthDU)
		{
			glyphWidthDU = static_cast<unsigned int>(widthDU);
		}
		if (heightDU > 0 && static_cast<unsigned int>(heightDU) > glyphHeightDU)
		{
			glyphHeightDU = static_cast<unsigned int>(heightDU);
		}

		if (-blackboxLeftDU > xOffsetDU)
		{
			xOffsetDU = -blackboxLeftDU;
		}
		if (-blackboxTopDU > yOffsetDU)
		{
			yOffsetDU = -blackboxTopDU;
		}
	}

	constexpr float cellBufferEm = 0.5f;

	const float cellWidthEm = glyphWidthDU * DUToEm + cellBufferEm;
	const float cellHeightEm = glyphHeightDU * DUToEm + cellBufferEm;
	const float glyphCellXOffsetEm = xOffsetDU * DUToEm + 0.5f * cellBufferEm;
	const float glyphCellYOffsetEm = yOffsetDU * DUToEm + 0.5f * cellBufferEm;

#pragma endregion

#pragma region Glyph Rendering

#pragma region Glyph Run

	Microsoft::WRL::ComPtr<ID3D11Texture2D> glyphRunTexture;
	{
		D3D11_TEXTURE2D_DESC desc = {
			.Width = jfaWidth,
			.Height = jfaHeight,
			.MipLevels = 1,
			.ArraySize = 1,
			.Format = DXGI_FORMAT_B8G8R8A8_UNORM,
			.SampleDesc = {.Count = 1, .Quality = 0 },
			.Usage = D3D11_USAGE_DEFAULT,
			.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE,
			.CPUAccessFlags = NULL,
			.MiscFlags = NULL
		};

		result = myDevice->CreateTexture2D(&desc, NULL, &glyphRunTexture);
		if (FAILED(result))
		{
			//TODO: Log
			return false;
		}
	}

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> glyphRynSRV;
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC desc = {
			.Format = DXGI_FORMAT_UNKNOWN,
			.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D,
			.Texture2D = D3D11_TEX2D_SRV{
				.MostDetailedMip = 0u,
				.MipLevels = 1u
			}
		};
		result = myDevice->CreateShaderResourceView(glyphRunTexture.Get(), &desc, &glyphRynSRV);
		if (FAILED(result))
		{
			//TODO: Log
			return false;
		}
	}

	Microsoft::WRL::ComPtr<IDXGISurface> surface;
	result = glyphRunTexture.As(&surface);
	if (FAILED(result))
	{
		//TODO: Log
		return false;
	}

	D2D1_RENDER_TARGET_PROPERTIES properties = {
		.type = D2D1_RENDER_TARGET_TYPE_DEFAULT,
		.pixelFormat = {
			.format = DXGI_FORMAT_B8G8R8A8_UNORM,
			.alphaMode = D2D1_ALPHA_MODE_PREMULTIPLIED
		},
		.dpiX = dpi,
		.dpiY = dpi,
		.usage = D2D1_RENDER_TARGET_USAGE_NONE,
		.minLevel = D2D1_FEATURE_LEVEL_DEFAULT
	};

	Microsoft::WRL::ComPtr<ID2D1RenderTarget> glyphRun2DRT;
	result = myFactory->CreateDxgiSurfaceRenderTarget(
		surface.Get(),
		&properties,
		&glyphRun2DRT
	);
	if (FAILED(result))
	{
		//TODO: Log
		return false;
	}

	D2D1_MATRIX_3X2_F transform = {};

	transform.m11 = 1.f;
	transform.m12 = 0.f;
	transform.m21 = 0.f;
	transform.m22 = 1.f;
	transform.dx = 0.f;
	transform.dy = 0.f;

	glyphRun2DRT->SetTransform(transform);

	Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> brush;
	result = glyphRun2DRT->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White, 1.f), &brush);
	if (FAILED(result))
	{
		//TODO: Log
		return false;
	}


#pragma region Atlas Setup

	unsigned int atlasWidth = 32;

	while ((static_cast<float>(atlasWidth) / (cellWidthEm * aFontSize)) * (static_cast<float>(atlasWidth) / (cellHeightEm * aFontSize)) < static_cast<unsigned int>(aNumCharacters))
	{
		atlasWidth *= 2;

		if (atlasWidth > D3D11_REQ_TEXTURE2D_U_OR_V_DIMENSION)
		{
			//TODO: Log
			return false;
		}
	}

	unsigned int atlasHeight = atlasWidth;

	while ((static_cast<float>(atlasWidth) / (cellWidthEm * aFontSize)) * (static_cast<float>(atlasHeight / 2) / (cellHeightEm * aFontSize)) >= static_cast<unsigned int>(aNumCharacters))
	{
		atlasHeight /= 2;
	}

	const unsigned int xCellCount = static_cast<unsigned int>(static_cast<float>(atlasWidth) / (cellWidthEm * aFontSize));

	Microsoft::WRL::ComPtr<ID3D11Texture2D> atlasTexture;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> atlasStagingTexture;
	{
		D3D11_TEXTURE2D_DESC desc = {
			.Width = atlasWidth,
			.Height = atlasHeight,
			.MipLevels = 1,
			.ArraySize = 1,
			.Format = DXGI_FORMAT_R32G32B32A32_FLOAT,
			.SampleDesc = {.Count = 1, .Quality = 0 },
			.Usage = D3D11_USAGE_DEFAULT,
			.BindFlags = D3D11_BIND_RENDER_TARGET,
			.CPUAccessFlags = NULL,
			.MiscFlags = NULL
		};

		result = myDevice->CreateTexture2D(&desc, NULL, &atlasTexture);
		if (FAILED(result))
		{
			//TODO: Log
			return false;
		}

		desc.Usage = D3D11_USAGE_STAGING;
		desc.BindFlags = NULL;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
		result = myDevice->CreateTexture2D(&desc, NULL, &atlasStagingTexture);
		if (FAILED(result))
		{
			//TODO: Log
			return false;
		}
	}

	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> atlasRTV;
	{
		D3D11_RENDER_TARGET_VIEW_DESC desc = {
			.Format = DXGI_FORMAT_UNKNOWN,
			.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D,
			.Texture2D = D3D11_TEX2D_RTV{
				.MipSlice = 0
			}
		};

		result = myDevice->CreateRenderTargetView(atlasTexture.Get(), &desc, &atlasRTV);
		if (FAILED(result))
		{
			//TODO: Log
			return false;
		}
	}

#pragma endregion

	const float jfaFontSize = (std::max)(jfaWidth / cellWidthEm, jfaHeight / cellHeightEm);
	const float jfaOriginX = glyphCellXOffsetEm * jfaFontSize;
	const float jfaOriginY = glyphCellYOffsetEm * jfaFontSize;

	for (size_t i = 0; i < aNumCharacters; i++)
	{
		const FLOAT glyphAdvance = static_cast<FLOAT>(glyphMetrics[i].advanceWidth) * DUToEm;
		const DWRITE_GLYPH_OFFSET glyphOffset = {
			.advanceOffset = 0.f,
			.ascenderOffset = 0.f
		};

		const float atlasOriginX = (static_cast<float>(i % xCellCount) * cellWidthEm * aFontSize + glyphCellXOffsetEm * aFontSize) / atlasWidth;
		const float atlasOriginY = (static_cast<float>(i / xCellCount) * cellHeightEm * aFontSize + glyphCellYOffsetEm * aFontSize) / atlasHeight;

		output.Glyphs[i].AtlasRect[0] = output.Glyphs[i].Bounds[0] * aFontSize / static_cast<float>(atlasWidth) + atlasOriginX;
		output.Glyphs[i].AtlasRect[1] = output.Glyphs[i].Bounds[1] * aFontSize / static_cast<float>(atlasHeight) + atlasOriginY;
		output.Glyphs[i].AtlasRect[2] = output.Glyphs[i].Bounds[2] * aFontSize / static_cast<float>(atlasWidth) + atlasOriginX;
		output.Glyphs[i].AtlasRect[3] = output.Glyphs[i].Bounds[3] * aFontSize / static_cast<float>(atlasHeight) + atlasOriginY;

		const DWRITE_GLYPH_RUN glyphRun = {
			.fontFace = fontFace.Get(),
			.fontEmSize = jfaFontSize,
			.glyphCount = 1,
			.glyphIndices = &glyphIndices[i],
			.glyphAdvances = &glyphAdvance,
			.glyphOffsets = &glyphOffset,
			.isSideways = FALSE,
			.bidiLevel = 0
		};

		glyphRun2DRT->BeginDraw();
		glyphRun2DRT->Clear();
		glyphRun2DRT->DrawGlyphRun(
			{ jfaOriginX, jfaOriginY },
			&glyphRun,
			brush.Get(),
			DWRITE_MEASURING_MODE_NATURAL
		);
		result = glyphRun2DRT->EndDraw();
		if (FAILED(result))
		{
			//TODO: Log
			return false;
		}

#pragma region JFA

#pragma region JFA State Setup

		myDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
		myDeviceContext->IASetInputLayout(myJFAInputLayout.Get());

		myDeviceContext->VSSetShader(myJFAVertexShader.Get(), NULL, 0);
		myDeviceContext->GSSetShader(myJFAGeometryShader.Get(), NULL, 0);

		myDeviceContext->PSSetShaderResources(0, 1, myJFASRV.GetAddressOf());

		{
			D3D11_VIEWPORT viewport = {
				.TopLeftX = 0.f,
				.TopLeftY = 0.f,
				.Width = static_cast<FLOAT>(jfaWidth),
				.Height = static_cast<FLOAT>(jfaHeight),
				.MinDepth = 0.f,
				.MaxDepth = 1.f
			};
			myDeviceContext->RSSetViewports(1, &viewport);
		}

#pragma endregion

		const float uvOriginX = (glyphCellXOffsetEm * jfaFontSize) / static_cast<float>(jfaWidth);
		const float uvOriginY = (glyphCellYOffsetEm * jfaFontSize) / static_cast<float>(jfaHeight);

		float uvBoundsRect[4] = {
			uvOriginX + output.Glyphs[i].Bounds[0] * jfaFontSize / static_cast<float>(jfaWidth),
			uvOriginY + output.Glyphs[i].Bounds[1] * jfaFontSize / static_cast<float>(jfaHeight),
			uvOriginX + output.Glyphs[i].Bounds[2] * jfaFontSize / static_cast<float>(jfaWidth),
			uvOriginY + output.Glyphs[i].Bounds[3] * jfaFontSize / static_cast<float>(jfaHeight)
		};

		{
			myDeviceContext->IASetVertexBuffers(0, 0, NULL, NULL, NULL);

			JFAVertex vertex = {
				.Rectangle = {
					uvBoundsRect[0],
					uvBoundsRect[1],
					uvBoundsRect[2],
					uvBoundsRect[3]
				},
				.UVRectangle = {
					uvBoundsRect[0],
					uvBoundsRect[1],
					uvBoundsRect[2],
					uvBoundsRect[3]
				}
			};
			UpdateBuffer(myDeviceContext.Get(), myJFAVertexBuffer.Get(), &vertex, sizeof(vertex));

			{
				UINT vertexStride = sizeof(JFAVertex);
				UINT offset = 0u;
				myDeviceContext->IASetVertexBuffers(0, 1, myJFAVertexBuffer.GetAddressOf(), &vertexStride, &offset);
			}
		}

		myDeviceContext->PSSetShader(myJFAInitPixelShader.Get(), NULL, 0);

		myDeviceContext->PSSetSamplers(0, 1, myJFAPointClampSamplerState.GetAddressOf());
		myDeviceContext->PSSetShaderResources(0, 1, glyphRynSRV.GetAddressOf());
		myDeviceContext->OMSetRenderTargets(1, myJFARTV.GetAddressOf(), NULL);
		myDeviceContext->Draw(1, 0);

		//{
		//	myDeviceContext->OMSetRenderTargets(0, NULL, NULL);
		//	std::wstring file;
		//	file += L"Glyph_";
		//	file += std::to_wstring(static_cast<unsigned int>(aCharacters[i]));
		//	file += L"_JFAInit.bmp";
		//	DebugExportTexture(myDevice.Get(), myDeviceContext.Get(), myJFATargetTexture.Get(), file.c_str());
		//}

		myDeviceContext->PSSetShader(myJFAPixelShader.Get(), NULL, 0);

		float stepDistance[2] = { 1.f, 1.f };
		const float minStepDistance[2] = { 1.f / static_cast<float>(jfaWidth), 1.f / static_cast<float>(jfaHeight) };
		size_t count = 0;
		while (stepDistance[0] >= minStepDistance[0] || stepDistance[1] >= minStepDistance[1])
		{
			myDeviceContext->OMSetRenderTargets(0, NULL, NULL);

			{
				ID3D11ShaderResourceView* dummy = NULL;
				myDeviceContext->PSSetShaderResources(0, 1, &dummy);
			}

			myDeviceContext->CopyResource(myJFAResourceTexture.Get(), myJFATargetTexture.Get());

			{
				ID3D11Buffer* dummy = NULL;
				myDeviceContext->PSSetConstantBuffers(0, 1, &dummy);

				JFABuffer jfaBuffer = {
					.StepDistance = {
						stepDistance[0],
						stepDistance[1]
					}
				};
				result = UpdateBuffer(myDeviceContext.Get(), myJFAConstantBuffer.Get(), &jfaBuffer, sizeof(jfaBuffer));
				if (FAILED(result))
				{
					//TODO: Log.
					continue;
				}
			}

			myDeviceContext->PSSetConstantBuffers(0, 1, myJFAConstantBuffer.GetAddressOf());

			myDeviceContext->OMSetRenderTargets(1, myJFARTV.GetAddressOf(), NULL);
			myDeviceContext->PSSetShaderResources(0, 1, myJFASRV.GetAddressOf());

			myDeviceContext->Draw(1, 0);

			stepDistance[0] *= 0.5f;
			stepDistance[1] *= 0.5f;

			//{
			//	std::wstring file;
			//	file += L"Glyph_";
			//	file += std::to_wstring(static_cast<unsigned int>(aCharacters[i]));
			//	file += L"_JFA_";
			//	file += std::to_wstring(count);
			//	file += L".bmp";
			//	DebugExportTexture(myDevice.Get(), myDeviceContext.Get(), myJFATargetTexture.Get(), file.c_str());
			//}

			++count;
		}
		{
			ID3D11Buffer* dummy = NULL;
			myDeviceContext->PSSetConstantBuffers(0, 1, &dummy);
		}

#pragma endregion

#pragma region Down Sampling
		//TODO: NEED to downsample in half increments so that pixels get proparly avaraged.

		{
			myDeviceContext->IASetVertexBuffers(0, 0, NULL, NULL, NULL);

			JFAVertex vertex = {
				.Rectangle = {
					uvBoundsRect[0],
					uvBoundsRect[1],
					uvBoundsRect[2],
					uvBoundsRect[3]
				},
				.UVRectangle = {
					uvBoundsRect[0],
					uvBoundsRect[1],
					uvBoundsRect[2],
					uvBoundsRect[3]
				}
			};
			result = UpdateBuffer(myDeviceContext.Get(), myJFAVertexBuffer.Get(), &vertex, sizeof(vertex));
			if (FAILED(result))
			{
				//TODO: Log.
				return false;
			}

			{
				UINT vertexStride = sizeof(JFAVertex);
				UINT offset = 0u;
				myDeviceContext->IASetVertexBuffers(0, 1, myJFAVertexBuffer.GetAddressOf(), &vertexStride, &offset);
			}
		}

		myDeviceContext->PSSetSamplers(0, 1, myJFATrilinearClampSamplerState.GetAddressOf());

		myDeviceContext->PSSetShader(myJFADownSamplePixelShader.Get(), NULL, 0);

		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> downSampleResource = myJFASRV;

		//{
		//	myDeviceContext->OMSetRenderTargets(0, NULL, NULL);

		//	{
		//		ID3D11ShaderResourceView* dummy = NULL;
		//		myDeviceContext->PSSetShaderResources(0, 1, &dummy);
		//	}

		//	std::wstring file;
		//	file += L"JFAResource_";
		//	file += std::to_wstring(static_cast<unsigned int>(aCharacters[i]));
		//	file += L".bmp";
		//	DebugExportTexture(myDevice.Get(), myDeviceContext.Get(), myJFAResourceTexture.Get(), file.c_str());
		//}

		float downSampleSizeFactor = 1.f;
		for (size_t j = 0; j < myDownSampleChain.size(); j++)
		{
			__DownsampleObjects& downSample = myDownSampleChain[j];

			downSampleSizeFactor *= 0.5f;

			unsigned int toWidth;
			unsigned int toHeight;
			{
				D3D11_TEXTURE2D_DESC desc = {};
				downSample.Texture->GetDesc(&desc);
				toWidth = desc.Width;
				toHeight = desc.Height;
			}

			unsigned int fromWidth = toWidth * 2;
			unsigned int fromHeight = toHeight * 2;

			{
				ID3D11Buffer* dummy = NULL;
				myDeviceContext->PSSetConstantBuffers(0, 1, &dummy);

				JFABuffer jfaBuffer = {
					.StepDistance = {
						1.f / ((uvBoundsRect[2] - uvBoundsRect[0]) * static_cast<float>(fromWidth)),
						1.f / ((uvBoundsRect[3] - uvBoundsRect[1]) * static_cast<float>(fromHeight)),
					}
				};
				result = UpdateBuffer(myDeviceContext.Get(), myJFAConstantBuffer.Get(), &jfaBuffer, sizeof(jfaBuffer));
				if (FAILED(result))
				{
					//TODO: Log.
					continue;
				}
			}

			{
				D3D11_VIEWPORT viewport = {
					.TopLeftX = 0.f,
					.TopLeftY = 0.f,
					.Width = static_cast<FLOAT>(toWidth),
					.Height = static_cast<FLOAT>(toHeight),
					.MinDepth = 0.f,
					.MaxDepth = 1.f
				};
				myDeviceContext->RSSetViewports(1, &viewport);
			}

			//FLOAT clearColor[4] = { 0.f, 0.f, 0.f, 0.f };
			//myDeviceContext->ClearRenderTargetView(downSample.RTV.Get(), clearColor);

			myDeviceContext->OMSetRenderTargets(1, downSample.RTV.GetAddressOf(), NULL);
			myDeviceContext->PSSetShaderResources(0, 1, downSampleResource.GetAddressOf());

			myDeviceContext->Draw(1, 0);

			downSampleResource = downSample.SRV;

			//{
			//	myDeviceContext->OMSetRenderTargets(0, NULL, NULL);
			//	{
			//		ID3D11ShaderResourceView* dummy = NULL;
			//		myDeviceContext->PSSetShaderResources(0, 1, &dummy);
			//	}

			//	std::wstring file;
			//	file += L"DownSample_";
			//	file += std::to_wstring(static_cast<unsigned int>(aCharacters[i]));
			//	file += L"_";
			//	file += std::to_wstring(j);
			//	file += L".bmp";
			//	DebugExportTexture(myDevice.Get(), myDeviceContext.Get(), downSample.Texture.Get(), file.c_str());
			//}

			// If the resulting texture is less than double the size of the fontsize, it is ready to be sampled. 
			if (toWidth <= 2.f * aFontSize && toHeight <= 2.f * aFontSize)
			{
				break;
			}
		}

#pragma endregion

		myDeviceContext->OMSetRenderTargets(0, NULL, NULL);

		{
			ID3D11ShaderResourceView* dummy = NULL;
			myDeviceContext->PSSetShaderResources(0, 1, &dummy);
		}

		//{
		//	std::wstring file;
		//	file += L"Glyph_";
		//	file += std::to_wstring(static_cast<unsigned int>(aCharacters[i]));
		//	file += L"_GlyphRun.bmp";
		//	DebugExportTexture(myDevice.Get(), myDeviceContext.Get(), glyphRunTexture.Get(), file.c_str());
		//}
		//{
		//	std::wstring file;
		//	file += L"Glyph_";
		//	file += std::to_wstring(static_cast<unsigned int>(aCharacters[i]));
		//	file += L"_JFA.bmp";
		//	DebugExportTexture(myDevice.Get(), myDeviceContext.Get(), myJFATargetTexture.Get(), file.c_str());
		//}

		{
			ID3D11Buffer* dummy = NULL;
			myDeviceContext->PSSetConstantBuffers(0, 1, &dummy);

			JFABuffer jfaBuffer = {
				.StepDistance = {
					0.f,
					0.f
				}
			};
			result = UpdateBuffer(myDeviceContext.Get(), myJFAConstantBuffer.Get(), &jfaBuffer, sizeof(jfaBuffer));
			if (FAILED(result))
			{
				//TODO: Log.
				continue;
			}
		}

		{
			D3D11_VIEWPORT viewport = {
				.TopLeftX = 0.f,
				.TopLeftY = 0.f,
				.Width = static_cast<FLOAT>(atlasWidth),
				.Height = static_cast<FLOAT>(atlasHeight),
				.MinDepth = 0.f,
				.MaxDepth = 1.f
			};
			myDeviceContext->RSSetViewports(1, &viewport);
		}

		//myDeviceContext->CopyResource(myJFAResourceTexture.Get(), myJFATargetTexture.Get());

		myDeviceContext->PSSetShader(myJFADownSamplePixelShader.Get(), NULL, 0);

		myDeviceContext->OMSetRenderTargets(1, atlasRTV.GetAddressOf(), NULL);
		myDeviceContext->PSSetShaderResources(0, 1, downSampleResource.GetAddressOf());

		{
			myDeviceContext->IASetVertexBuffers(0, 0, NULL, NULL, NULL);

			assets::Font::Glyph& glyph = output.Glyphs[i];


			JFAVertex vertex = {
				.Rectangle = {
					glyph.AtlasRect[0],
					glyph.AtlasRect[1],
					glyph.AtlasRect[2],
					glyph.AtlasRect[3]
				},
				.UVRectangle = {
					uvBoundsRect[0],
					uvBoundsRect[1],
					uvBoundsRect[2],
					uvBoundsRect[3]
					//0.f, 0.f, 1.f, 1.f
					//downSampleOriginX + output.Glyphs[i].Bounds[0] * downSampleSizeFactor * jfaFontSize / static_cast<float>(jfaWidth * downSampleSizeFactor),
					//downSampleOriginY + output.Glyphs[i].Bounds[1] * downSampleSizeFactor * jfaFontSize / static_cast<float>(jfaHeight * downSampleSizeFactor),
					//downSampleOriginX + output.Glyphs[i].Bounds[2] * downSampleSizeFactor * jfaFontSize / static_cast<float>(jfaWidth * downSampleSizeFactor),
					//downSampleOriginY + output.Glyphs[i].Bounds[3] * downSampleSizeFactor * jfaFontSize / static_cast<float>(jfaHeight * downSampleSizeFactor)
				}
			};
			D3D11_SUBRESOURCE_DATA data = {
				.pSysMem = &vertex,
				.SysMemPitch = 0u,
				.SysMemSlicePitch = 0u
			};

			D3D11_MAPPED_SUBRESOURCE mappedVertexBuffer = {};
			result = myDeviceContext->Map(myJFAVertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, NULL, &mappedVertexBuffer);
			if (FAILED(result))
			{
				//TODO: Log.
				continue;
			}
			memcpy_s(mappedVertexBuffer.pData, mappedVertexBuffer.DepthPitch, &vertex, sizeof(vertex));
			myDeviceContext->Unmap(myJFAVertexBuffer.Get(), 0);
			{
				UINT vertexStride = sizeof(JFAVertex);
				UINT offset = 0u;
				myDeviceContext->IASetVertexBuffers(0, 1, myJFAVertexBuffer.GetAddressOf(), &vertexStride, &offset);
			}
		}

		myDeviceContext->Draw(1, 0);

		//{
		//	myDeviceContext->OMSetRenderTargets(0, NULL, NULL);
		//	std::wstring file;
		//	file += L"Atlas_";
		//	file += std::to_wstring(static_cast<unsigned int>(aCharacters[i]));
		//	file += L".bmp";
		//	DebugExportTexture(myDevice.Get(), myDeviceContext.Get(), atlasTexture.Get(), file.c_str());
		//}
	}

	myDeviceContext->OMSetRenderTargets(0, NULL, NULL);

	{
		ID3D11ShaderResourceView* dummy = NULL;
		myDeviceContext->PSSetShaderResources(0, 1, &dummy);
	}

#pragma endregion

#pragma endregion

#pragma region Finalize Font Atlas

	{
		DebugExportTexture(myDevice.Get(), myDeviceContext.Get(), atlasTexture.Get(), L"Atlas_Texture.bmp");
	}

	myDeviceContext->CopyResource(atlasStagingTexture.Get(), atlasTexture.Get());

	D3D11_MAPPED_SUBRESOURCE mappedResource = {};
	result = myDeviceContext->Map(atlasStagingTexture.Get(), 0, D3D11_MAP_READ, NULL, &mappedResource);
	if (FAILED(result))
	{
		//TODO: Log
		return false;
	}

	output.Atlas.Width = static_cast<unsigned short>(atlasWidth);
	output.Atlas.Height = static_cast<unsigned short>(atlasHeight);
	output.Atlas.RowPitch = mappedResource.RowPitch;
	output.Atlas.DepthPitch = mappedResource.DepthPitch;
	output.Atlas.Data.resize(mappedResource.DepthPitch);
	errno_t error = memcpy_s(output.Atlas.Data.data(), output.Atlas.Data.size() * sizeof(output.Atlas.Data[0]), mappedResource.pData, mappedResource.DepthPitch);

	myDeviceContext->Unmap(atlasStagingTexture.Get(), 0);

	if (error != 0)
	{
		//TODO: Log
		return false;
	}

	std::ofstream file(aTarget, std::ios::binary);

	CommonUtilities::StreamWriter writer(file);
	CommonUtilities::Serializer serializer;
	if (not serializer.Write(assets::FONT_FILE_HEADER, writer))
	{
		//TODO: Log
		return false;
	}
	if (not serializer.Write(output, writer))
	{
		//TODO: Log
		return false;
	}

	return true;

#pragma endregion
}
