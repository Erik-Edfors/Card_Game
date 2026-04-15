#include "FontBuilder.h"

#include <Core/CommonUtilities/Serialization/StreamWrapper.hpp>
#include <Core/CommonUtilities/Serialization/Serializer.hpp>
#include <Core/Assets/Font.h>

#include <fstream>
#include <iostream>

struct JFAVertex
{
	float Rectangle[4];
};

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

FontBuilder::FontBuilder()
{
}
FontBuilder::~FontBuilder()
{
}

bool FontBuilder::Initialize()
{
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

		const D3D11_INPUT_ELEMENT_DESC elements[1] = {
			D3D11_INPUT_ELEMENT_DESC{
				.SemanticName = "RECTANGLE",
				.SemanticIndex = 0,
				.Format = DXGI_FORMAT_R32G32B32A32_FLOAT,
				.InputSlot = 0u,
				.AlignedByteOffset = 0xffffffff,
				.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA,
				.InstanceDataStepRate = 0u
			}
		};

		result = myDevice->CreateInputLayout(elements, 1, byteCode, byteCodeSize, &myJFAInputLayout);
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

		result = myDevice->CreateSamplerState(&desc, &myJFASamplerState);
		if (FAILED(result))
		{
			//TODO: Log
			return false;
		}
	}

	{
		D3D11_TEXTURE2D_DESC desc = {
			.Width = 4096,
			.Height = 4096,
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
		JFAVertex vertex = {
			.Rectangle = { 0.f, 0.f, 1.f, 1.f }
		};

		D3D11_BUFFER_DESC desc = {
			.ByteWidth = sizeof(vertex),
			.Usage = D3D11_USAGE_IMMUTABLE,
			.BindFlags = D3D11_BIND_VERTEX_BUFFER,
			.CPUAccessFlags = NULL,
			.MiscFlags = NULL,
			.StructureByteStride = sizeof(vertex)
		};

		D3D11_SUBRESOURCE_DATA data = {
			.pSysMem = &vertex,
			.SysMemPitch = 0u,
			.SysMemSlicePitch = 0u
		};

		result = myDevice->CreateBuffer(&desc, &data, &myJFAVertexBuffer);
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
	const std::filesystem::path sourceAbsolute = (std::filesystem::current_path() / aSource).lexically_normal();

	D3D11_TEXTURE2D_DESC jfaTextureDesc = {};
	myJFATargetTexture->GetDesc(&jfaTextureDesc);

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
			.SampleDesc = { .Count = 1, .Quality = 0 },
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

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv;
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC desc = {
			.Format = DXGI_FORMAT_UNKNOWN,
			.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D,
			.Texture2D = D3D11_TEX2D_SRV{
				.MostDetailedMip = 0u,
				.MipLevels = 1u
			}
		};
		result = myDevice->CreateShaderResourceView(texture.Get(), &desc, &srv);
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


#pragma region JFA setup

	D3D11_VIEWPORT viewport = {
		.TopLeftX = 0.f,
		.TopLeftY = 0.f,
		.Width = static_cast<FLOAT>(jfaTextureDesc.Width),
		.Height = static_cast<FLOAT>(jfaTextureDesc.Height),
		.MinDepth = 0.f,
		.MaxDepth = 1.f
	};

	myDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	myDeviceContext->IASetInputLayout(myJFAInputLayout.Get());

	myDeviceContext->VSSetShader(myJFAVertexShader.Get(), NULL, 0);
	myDeviceContext->GSSetShader(myJFAGeometryShader.Get(), NULL, 0);

	myDeviceContext->PSSetShaderResources(0, 1, myJFASRV.GetAddressOf());
	myDeviceContext->OMSetRenderTargets(1, myJFARTV.GetAddressOf(), NULL);
	myDeviceContext->RSSetViewports(1, &viewport);
	myDeviceContext->PSSetSamplers(0, 1, myJFASamplerState.GetAddressOf());

	UINT vertexStride = sizeof(JFAVertex);
	UINT offset = 0u;
	myDeviceContext->IASetVertexBuffers(0, 1, myJFAVertexBuffer.GetAddressOf(), &vertexStride, &offset);

#pragma endregion


	const float originX = jfaTextureDesc.Width * glyphCellXOffset / cellWidth;
	const float originY = jfaTextureDesc.Height * glyphCellYOffset / cellHeight;
	for (size_t i = 0; i < aNumCharacters; i++)
	{
		renderTarget->BeginDraw();

		const FLOAT glyphAdvance = static_cast<FLOAT>(glyphMetrics[i].advanceWidth) * DUToEm;
		const DWRITE_GLYPH_OFFSET glyphOffset = {
			.advanceOffset = 0.f,
			.ascenderOffset = 0.f
		};

		output.Glyphs[i].AtlasRect[0] = (output.Glyphs[i].Bounds[0] + originX) / static_cast<float>(textureWidth);
		output.Glyphs[i].AtlasRect[1] = (output.Glyphs[i].Bounds[1] + originY) / static_cast<float>(textureHeight);
		output.Glyphs[i].AtlasRect[2] = (output.Glyphs[i].Bounds[2] + originX) / static_cast<float>(textureWidth);
		output.Glyphs[i].AtlasRect[3] = (output.Glyphs[i].Bounds[3] + originY) / static_cast<float>(textureHeight);

		const DWRITE_GLYPH_RUN glyphRun = {
			.fontFace = fontFace.Get(),
			.fontEmSize = static_cast<FLOAT>((std::min)(jfaTextureDesc.Width, jfaTextureDesc.Height)) * (1.f - cellBuffer / cellWidth),
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
		result = renderTarget->EndDraw();

#pragma region JFA

		myDeviceContext->PSSetShader(myJFAInitPixelShader.Get(), NULL, 0);

		myDeviceContext->PSSetShaderResources(0, 1, srv.GetAddressOf());
		myDeviceContext->Draw(1, 0);

		myDeviceContext->PSSetShader(myJFAPixelShader.Get(), NULL, 0);

		float stepDistance = 1.f;
		const float minStepDistance = 1.f / static_cast<float>((std::max)(jfaTextureDesc.Width, jfaTextureDesc.Height));
		while (stepDistance > minStepDistance)
		{
			myDeviceContext->OMSetRenderTargets(0, NULL, NULL);

			{
				ID3D11ShaderResourceView* dummy = NULL;
				myDeviceContext->PSSetShaderResources(0, 1, &dummy);
			}

			myDeviceContext->CopyResource(myJFAResourceTexture.Get(), myJFATargetTexture.Get());


			myDeviceContext->OMSetRenderTargets(1, myJFARTV.GetAddressOf(), NULL);
			myDeviceContext->PSSetShaderResources(0, 1, myJFASRV.GetAddressOf());

			myDeviceContext->Draw(1, 0);

			stepDistance *= 0.5f;
		}

#pragma endregion
	}
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
}
