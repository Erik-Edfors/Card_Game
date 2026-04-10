#include "FontBuilder.h"
#include <fstream>

#include <iostream>

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

	return true;
}

bool FontBuilder::Build(const std::filesystem::path& aSource, const std::filesystem::path& aTarget, unsigned int aFontSize, const wchar_t* aCharacters, size_t aNumCharacters)
{
	std::filesystem::path sourceAbsolute = (std::filesystem::current_path() / aSource).lexically_normal();

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

	constexpr float dpi = 92.f;
	const FLOAT DUToEm = 1.f / static_cast<FLOAT>(fontMetrics.designUnitsPerEm);
	const float EmToPx = static_cast<float>(aFontSize);
	//const float PxToEm = 1.f / EmToPx;


	UINT32* codePoints = new UINT32[aNumCharacters];
	UINT16* glyphIndices = new UINT16[aNumCharacters];
	DWRITE_GLYPH_METRICS* glyphMetrics = new DWRITE_GLYPH_METRICS[aNumCharacters];

	for (size_t i = 0; i < aNumCharacters; i++)
	{
		codePoints[i] = aCharacters[i];
	}

	result = fontFace->GetGlyphIndicesW(codePoints, static_cast<UINT32>(aNumCharacters), glyphIndices);
	if (FAILED(result))
	{
		delete[] codePoints;
		delete[] glyphIndices;
		delete[] glyphMetrics;
		//TODO: Log
		return false;
	}

	result = fontFace->GetDesignGlyphMetrics(glyphIndices, static_cast<UINT32>(aNumCharacters), glyphMetrics, FALSE);
	if (FAILED(result))
	{
		delete[] codePoints;
		delete[] glyphIndices;
		delete[] glyphMetrics;
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
		int blockboxBottom = glyphMetrics[i].verticalOriginY + glyphMetrics[i].advanceHeight - glyphMetrics[i].bottomSideBearing;

		int width = blackboxRight - blackboxLeft;
		int height = blockboxBottom - blackboxTop;

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

	Microsoft::WRL::ComPtr<ID3D11Texture2D> texture;
	result = myDevice->CreateTexture2D(
		&desc,
		NULL,
		&texture
	);
	if (FAILED(result))
	{
		//TODO: Log
		return false;
	}

	desc.Usage = D3D11_USAGE_STAGING;
	desc.BindFlags = NULL;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> stagingTexture;
	result = myDevice->CreateTexture2D(
		&desc,
		NULL,
		&stagingTexture
	);
	if (FAILED(result))
	{
		//TODO: Log
		return false;
	}

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
			.alphaMode = D2D1_ALPHA_MODE_IGNORE
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

	Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> brush;
	result = renderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &brush);
	if (FAILED(result))
	{
		//TODO: Log
		return false;
	}



	renderTarget->BeginDraw();
	for (size_t i = 0; i < aNumCharacters; i++)
	{
		FLOAT glyphAdvance = static_cast<FLOAT>(glyphMetrics[i].advanceWidth) * DUToEm;
		DWRITE_GLYPH_OFFSET glyphOffset = {
			.advanceOffset = 0.f,
			.ascenderOffset = 0.f
		};

		float originX = static_cast<float>(i % xCellCount) * cellWidth + glyphCellXOffset;
		float originY = static_cast<float>(i / xCellCount) * cellHeight + glyphCellYOffset;

		DWRITE_GLYPH_RUN glyphRun = {
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

	delete[] codePoints;
	delete[] glyphIndices;
	delete[] glyphMetrics;

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

	const unsigned int imageSize = desc.Height * ((desc.Width * 3 + 3) & 0xfffffffc);
	constexpr unsigned int stride = 4;
	constexpr unsigned int bmpStride = 3;

	char* imageData = new char[imageSize];

	for (unsigned int y = 0; y < desc.Height; y++)
	{
		for (unsigned int x = 0; x < desc.Width; x++)
		{
			const unsigned int resourceIndex = y * mappedResource.RowPitch + x * stride;
			const unsigned int bmpIndex = y * ((desc.Width * 3 + 3) & 0xfffffffc) + x * bmpStride;

			imageData[bmpIndex + 0] = reinterpret_cast<char*>(mappedResource.pData)[resourceIndex + 2];
			imageData[bmpIndex + 1] = reinterpret_cast<char*>(mappedResource.pData)[resourceIndex + 1];
			imageData[bmpIndex + 2] = reinterpret_cast<char*>(mappedResource.pData)[resourceIndex + 0];
		}
	}

	myDeviceContext->Unmap(stagingTexture.Get(), 0);

	const float pixelsPerMeter = dpi * (1.f / 0.0254f);

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

	std::ofstream file(aTarget);

	file.write(reinterpret_cast<char*>(&bmpFileHeader), sizeof(bmpFileHeader));
	file.write(reinterpret_cast<char*>(&bmpHeader), sizeof(bmpHeader));
	file.write(imageData, imageSize);

	return true;
}
