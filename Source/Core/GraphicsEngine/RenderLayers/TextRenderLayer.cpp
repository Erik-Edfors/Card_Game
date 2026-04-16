#include "GraphicsEngine.pch.h"
#include "Common.h"
#include "TextRenderLayer.h"
#include "ConstantBuffers/FrameBuffer.h"
#include "ConstantBuffers/ObjectBuffer.h"
#include "ConstantBuffers/MaterialBuffer.h"

#include <Core/RenderHardwareInterface/Device.h>
#include <Core/RenderHardwareInterface/Objects/Texture2D.h>
#include <Core/RenderHardwareInterface/Objects/ShaderResource.h>
#include <Core/RenderHardwareInterface/Objects/SamplerState.h>
#include <Core/RenderHardwareInterface/Objects/BlendState.h>
#include <Core/RenderHardwareInterface/Objects/InputLayout.h>
#include <Core/RenderHardwareInterface/Objects/DeferredDeviceContext.h>
#include <Core/RenderHardwareInterface/Objects/RenderTarget.h>
#include <Core/RenderHardwareInterface/Objects/DepthStencil.h>
#include <Core/RenderHardwareInterface/Objects/Buffers/ConstantBuffer.h>
#include <Core/RenderHardwareInterface/Objects/Buffers/VertexBuffer.h>

#include <Core/RenderHardwareInterface/Objects/Shaders/VertexShader.h>
#include <Core/RenderHardwareInterface/Objects/Shaders/GeometryShader.h>
#include <Core/RenderHardwareInterface/Objects/Shaders/PixelShader.h>

#include <Core/GraphicsEngine/GraphicsEngine.h>

#include <Core/Assets/Font.h>
#include <Core/CommonUtilities/Serialization/StreamWrapper.hpp>
#include <Core/CommonUtilities/Serialization/Serializer.hpp>

#include <cwctype>

#ifdef CreateFont
#undef CreateFont
#endif

struct GlyphVertex
{
	CommonUtilities::AABB2Df Bounds;
	CommonUtilities::AABB2Df TextureRect;
};

Font::Font() :
	myFontSize(0),
	myUnderlineHeight(0.f),
	myUnderlineThickness(0.f),
	myStrikethroughHeight(0.f),
	myStrikethroughThickness(0.f),
	myAdvanceHeight(0.f)
{
}

Font::~Font()
{
}

const Font::Glyph* Font::FindGlyph(wchar_t aChar) const noexcept
{
	auto it = myGlyphMap.find(aChar);
	if (it != myGlyphMap.end())
	{
		return &it->second;
	}
	return nullptr;
}

float Font::GetFontSize() const noexcept
{
	return myFontSize;
}

float Font::GetAdvanceHeight() const noexcept
{
	return myAdvanceHeight;
}

float Font::GetUnderlineHeight() const noexcept
{
	return myUnderlineHeight;
}

float Font::GetUnderlineThickness() const noexcept
{
	return myUnderlineThickness;
}

float Font::GetStrikethroughHeight() const noexcept
{
	return myStrikethroughHeight;
}

float Font::GetStrikethroughThickness() const noexcept
{
	return myStrikethroughThickness;
}

graphics::ShaderResource* Font::GetAtlas() const noexcept
{
	return myAtlas.Get();
}

Text::Text() :
	myVertexCount(0)
{
}

Text::~Text()
{
}

void Text::Initialize(graphics::VertexBuffer* aVertexBuffer, size_t aVertexCount, graphics::ShaderResource* aAtlas, CommonUtilities::Vector2f aSize)
{
	myVertexBuffer = aVertexBuffer;
	myVertexCount = aVertexCount;
	myAtlas = aAtlas;
	mySize = aSize;
}

TextRenderLayer::TextRenderLayer()
{
}

TextRenderLayer::~TextRenderLayer()
{
}

Text TextRenderLayer::CreateText(const wchar_t* aText, const Font& aFont, const CommonUtilities::Vector4f& aColor, float aRoughness, float aMetallicity) const
{
	//TODO: allow setting material.
	aColor;
	aMetallicity;
	aRoughness;

	if (aText == nullptr)
	{
		return {};
	}

	size_t length = 0;
	while (aText[length] != L'\0') ++length;

	size_t glyphCount = 0;
	std::unique_ptr<GlyphVertex[]> glyphs = std::make_unique<GlyphVertex[]>(length);

	CommonUtilities::Vector2f cursor = { 0.f, 0.f };
	for (size_t i = 0; i < length; i++)
	{
		const wchar_t c = aText[i];

		const Font::Glyph* glyph = aFont.FindGlyph(c);

		const CommonUtilities::Vector2f position = cursor;

		switch (c)
		{
			case L'\n':
				cursor.y -= aFont.GetAdvanceHeight() * aFont.GetFontSize();
				cursor.x = 0.f;
				break;
			case L'\t':
				cursor.x += 2.f * aFont.GetFontSize() - std::fmodf(cursor.x, 2.f * aFont.GetFontSize());
				break;
		}

		if (not std::iswprint(c) || glyph == nullptr)
		{
			continue;
		}
		cursor.x += glyph->Advance * aFont.GetFontSize();

		if (std::iswblank(c))
		{
			continue;
		}

		GlyphVertex& vertex = glyphs[glyphCount++];
		vertex.Bounds.Min = position + CommonUtilities::Vector2f(glyph->Bounds[0], -glyph->Bounds[3]) * aFont.GetFontSize();
		vertex.Bounds.Max = position + CommonUtilities::Vector2f(glyph->Bounds[2], -glyph->Bounds[1]) * aFont.GetFontSize();
		vertex.TextureRect = {
			{ glyph->AtlasRect[0], glyph->AtlasRect[1] },
			{ glyph->AtlasRect[2], glyph->AtlasRect[3] }
		};
	}

	graphics::VertexBufferDesc desc = {
		.Name = "Text VertexBuffer",
		.Data = glyphs.get(),
		.Size = glyphCount * sizeof(glyphs[0]),
		.Usage = graphics::GRAPHICS_USAGE_IMMUTABLE,
		.CPUAccessFlags = graphics::GRAPHICS_CPU_ACCESS_NONE
	};

	graphics::PImpl<graphics::VertexBuffer> vertexBuffer;
	graphics::result::Code result = GetGraphicsEngine()->GetDevice()->CreateVertexBuffer(&vertexBuffer, &desc);
	if (GRAPHICS_FAILED(result))
	{
		//TODO: Log
		return {};
	}

	Text text;
	//TODO: Actually calculate size. And add an additional advance width at end.
	text.Initialize(vertexBuffer.Get(), glyphCount, aFont.GetAtlas(), cursor);

	return text;
}

Font TextRenderLayer::CreateFont(const wchar_t* aFontFile)
{
	std::ifstream file(aFontFile, std::ios::binary);

	if (not file.is_open())
	{
		return {};
	}

	CommonUtilities::StreamReader reader(file);
	reader.SetConstraints(
		CommonUtilities::ReadConstraints{
			.MaxSize = 100'000'000	// 100 MB size limit
		}
	);

	CommonUtilities::Serializer serializer;

	assets::FileHeader header;
	if (not serializer.Read(header, reader))
	{
		//TODO: Log
		return {};
	}

	if (memcmp(&header, &assets::FONT_FILE_HEADER, sizeof(header)) != 0)
	{
		//TODO: log
		return {};
	}

	assets::Font font;
	if (not serializer.Read(font, reader))
	{
		//TODO: Log
		return {};
	}

	graphics::PImpl<graphics::Texture2D> atlasTexture;
	{
		graphics::TextureSubresourceData subResource{
			.Data = font.Atlas.Data.data(),
			.RowPitch = font.Atlas.RowPitch,
			.DepthPitch = font.Atlas.DepthPitch
		};

		graphics::Texture2DDesc desc = {
			.BindFlags = graphics::GRAPHICS_BIND_SHADER_RESOURCE,
			.ResourceFlags = graphics::GRAPHICS_RESOURCE_NONE,
			.Name = "Font Atlas",	//TODO: Actually use font name.
			.Width = font.Atlas.Width,
			.Height = font.Atlas.Height,
			.MipLevels = 1,
			.NumTextures = 1,
			.Data = &subResource,
			.NumData = 1,
			.Format = graphics::GRAPHICS_FORMAT_R32G32B32A32_FLOAT,
			.Usage = graphics::GRAPHICS_USAGE_IMMUTABLE,
			.CPUAccessFlags = graphics::GRAPHICS_CPU_ACCESS_NONE
		};
		HRESULT result = GetGraphicsEngine()->GetDevice()->CreateTexture2D(&atlasTexture, &desc);
		if (FAILED(result))
		{
			//TODO: Log
			return {};
		}
	}

	graphics::PImpl<graphics::ShaderResource> atlasResource;
	{
		graphics::ShaderResourceDesc desc = {
			.Name = "Font Atlas ShaderResource",
			.Texture = atlasTexture.Get(),
			.Format = graphics::GRAPHICS_FORMAT_UNKNOWN,
			.ViewDimension = graphics::GRAPHICS_SHADER_RESOURCE_VIEW_DIMENSION_TEXTURE2D,
			.FirstSlice = 0,
			.NumSlices = 1,
			.MipLevels = 1,
			.MostDetailedMip = 0
		};
		HRESULT result = GetGraphicsEngine()->GetDevice()->CreateShaderResource(&atlasResource, &desc);
		if (FAILED(result))
		{
			//TODO: Log
			return {};
		}
	}

	Font result;
	result.myAtlas = atlasResource.Get();

	result.myAdvanceHeight = font.AdvanceHeight;
	result.myFontSize = font.FontSize;
	result.myUnderlineHeight = font.UnderlineHeight;
	result.myUnderlineThickness = font.UnderlineThickness;
	result.myStrikethroughHeight = font.StrikethroughHeight;
	result.myStrikethroughThickness = font.StrikethroughThickness;

	for (const assets::Font::Glyph& glyph : font.Glyphs)
	{
		result.myGlyphMap[glyph.Character] = Font::Glyph{
			.Bounds = {
				glyph.Bounds[0],
				glyph.Bounds[1],
				glyph.Bounds[2],
				glyph.Bounds[3]
			},
			.AtlasRect = {
				glyph.AtlasRect[0],
				glyph.AtlasRect[1],
				glyph.AtlasRect[2],
				glyph.AtlasRect[3]
			},
			.Advance = glyph.Advance
		};
	}

	return result;
}

void TextRenderLayer::Draw(const Text& aText, const CommonUtilities::Matrix4x4f& aTransform)
{
	myTexts.push_back(
		TextInstance{
			.Transform = aTransform,
			.Text = aText
		}
	);
}

void TextRenderLayer::SetTargets(graphics::RenderTarget* aRenderTarget, graphics::DepthStencil* aDepthStencil)
{
	myRenderTarget = aRenderTarget;
	myDepthStencil = aDepthStencil;
}

void TextRenderLayer::SetView(const CommonUtilities::Matrix4x4f& aView) noexcept
{
	myView = aView;
}

void TextRenderLayer::SetProjection(const CommonUtilities::Matrix4x4f& aProjection) noexcept
{
	myProjection = aProjection;
}

void TextRenderLayer::OnInit()
{

	{
		//TODO: use asset system or something to get shader file.
		std::ifstream file("../../Assets/Shaders/Text_VS.cso", std::ios::binary);

		size_t shaderSize = 0;

		void* shaderBuffer = nullptr;
		LoadStreamIntoMemory(&shaderBuffer, &shaderSize, file);

		GetGraphicsEngine()->GetDevice()->CreateShader(
			&myVertexShader,
			"Text VertexShader",
			shaderBuffer,
			shaderSize
		);


		const graphics::InputLayoutDesc desc = {
			.Name = "Text InputLayout",
			.LayoutDefinition = {
				graphics::LayoutElementDesc{
					.SemanticName = "BOUNDS",
					.Format = graphics::GRAPHICS_FORMAT_R32G32B32A32_FLOAT
				},
				graphics::LayoutElementDesc{
					.SemanticName = "UVRECT",
					.Format = graphics::GRAPHICS_FORMAT_R32G32B32A32_FLOAT
				}
			},
			.ShaderByteCode = shaderBuffer,
			.NumBytes = shaderSize
		};
		GetGraphicsEngine()->GetDevice()->CreateInputLayout(&myInputLayout, &desc);

		delete[] shaderBuffer;
	}
	{
		//TODO: use asset system or something to get shader file.
		std::ifstream file("../../Assets/Shaders/Text_GS.cso", std::ios::binary);

		size_t shaderSize = 0;

		void* shaderBuffer = nullptr;
		LoadStreamIntoMemory(&shaderBuffer, &shaderSize, file);

		GetGraphicsEngine()->GetDevice()->CreateShader(
			&myGeometryShader,
			"Text GeometryShader",
			shaderBuffer,
			shaderSize
		);

		delete[] shaderBuffer;
	}
	{
		std::ifstream file("../../Assets/Shaders/Text_PS.cso", std::ios::binary);

		size_t shaderSize = 0;

		void* shaderBuffer = nullptr;
		LoadStreamIntoMemory(&shaderBuffer, &shaderSize, file);

		GetGraphicsEngine()->GetDevice()->CreateShader(
			&myPixelShader,
			"Text PixelShader",
			shaderBuffer,
			shaderSize
		);

		delete[] shaderBuffer;
	}

	{
		const graphics::ConstantBufferDesc desc = {
			.Name = "Text FrameBuffer",
			.InitialData = nullptr,
			.Size = sizeof(FrameBuffer),
			.Usage = graphics::GRAPHICS_USAGE_DYNAMIC,
			.CPUAccessFlags = graphics::GRAPHICS_CPU_ACCESS_WRITE
		};
		GetGraphicsEngine()->GetDevice()->CreateConstantBuffer(&myFrameBuffer, &desc);
	}
	{
		const graphics::ConstantBufferDesc desc = {
			.Name = "Text Object Buffer",
			.InitialData = nullptr,
			.Size = sizeof(ObjectBuffer),
			.Usage = graphics::GRAPHICS_USAGE_DYNAMIC,
			.CPUAccessFlags = graphics::GRAPHICS_CPU_ACCESS_WRITE
		};
		GetGraphicsEngine()->GetDevice()->CreateConstantBuffer(&myObjectBuffer, &desc);
	}
	{
		const graphics::ConstantBufferDesc desc = {
			.Name = "Text Material Buffer",
			.InitialData = nullptr,
			.Size = sizeof(MaterialBuffer),
			.Usage = graphics::GRAPHICS_USAGE_DYNAMIC,
			.CPUAccessFlags = graphics::GRAPHICS_CPU_ACCESS_WRITE
		};
		GetGraphicsEngine()->GetDevice()->CreateConstantBuffer(&myMaterialBuffer, &desc);
	}

	{
		const graphics::BlendStateDesc desc = {
			.Name = "Text Blend State",
			.EnableAlphaToCoverage = false,
			.EnableIndependentBlend = false,
			.RenderTargetBlendDescriptions = {
				graphics::RenderTargetBlendDesc{
					.EnableBlend = true,
					.SourceBlend = graphics::GRAPHICS_BLEND_SRC_ALPHA,
					.DestinationBlend = graphics::GRAPHICS_BLEND_INV_SRC_ALPHA,
					.BlendOperation = graphics::GRAPHICS_BLEND_OPERATION_ADD,
					.AlphaSourceBlend = graphics::GRAPHICS_BLEND_SRC_ALPHA,
					.AlphaDestinationBlend = graphics::GRAPHICS_BLEND_INV_SRC_ALPHA,
					.AlphaBlendOperation = graphics::GRAPHICS_BLEND_OPERATION_ADD,
					.RenderTargetWriteMask = 0b1111
				}
			}
		};
		GetGraphicsEngine()->GetDevice()->CreateBlendState(&myBlendState, &desc);
	}

	{
		const graphics::SamplerStateDesc desc = {
			.Name = "Text Atlas Sampler",
			.Filter = graphics::GRAPHICS_SAMPLER_FILTER_MIN_MAG_POINT_MIP_LINEAR,
			.AddressU = graphics::GRAPHICS_TEXTURE_ADDRESS_CLAMP,
			.AddressV = graphics::GRAPHICS_TEXTURE_ADDRESS_CLAMP,
			.AddressW = graphics::GRAPHICS_TEXTURE_ADDRESS_CLAMP,
			.MipLODBias = 0.f,
			.MaxAnistropy = 0,
			.Comparison = graphics::GRAPHICS_COMPARISON_NEVER,
			.BorderColor = { 0.f, 0.f, 0.f, 0.f },
			.MinLOD = 0,
			.MaxLOD = FLT_MAX
		};
		GetGraphicsEngine()->GetDevice()->CreateSamplerState(&mySamplerState, &desc);
	}
}

void TextRenderLayer::OnRender()
{
	GetContext()->ResetState();

	GetContext()->PushEvent("Text Rendering");

	//const float clearColor[4] = { 0.f, 0.f, 0.f, 0.f };
	//GetContext()->ClearRenderTarget(myRenderTarget.Get(), clearColor);

	GetContext()->SetTopology(graphics::GRAPHICS_TOPOLOGY_POINTLIST);

	graphics::ConstantBuffer* buffers[] = {
		myFrameBuffer.Get(),
		myObjectBuffer.Get(),
		myMaterialBuffer.Get()
	};
	GetContext()->BindConstantBuffers(buffers, 3, 0, graphics::GRAPHICS_SHADER_STAGE_VERTEX | graphics::GRAPHICS_SHADER_STAGE_PIXEL);

	const CommonUtilities::Vector4f cameraPosition = myView.GetInverse().Row(4);

	const FrameBuffer frameBuffer = {
		.View = {
			{ myView(1, 1), myView(1, 2), myView(1, 3), myView(1, 4) },
			{ myView(2, 1), myView(2, 2), myView(2, 3), myView(2, 4) },
			{ myView(3, 1), myView(3, 2), myView(3, 3), myView(3, 4) },
			{ myView(4, 1), myView(4, 2), myView(4, 3), myView(4, 4) }
		},
		.Projection = {
			{ myProjection(1, 1), myProjection(1, 2), myProjection(1, 3), myProjection(1, 4) },
			{ myProjection(2, 1), myProjection(2, 2), myProjection(2, 3), myProjection(2, 4) },
			{ myProjection(3, 1), myProjection(3, 2), myProjection(3, 3), myProjection(3, 4) },
			{ myProjection(4, 1), myProjection(4, 2), myProjection(4, 3), myProjection(4, 4) }
		},
		.CameraPosition = { cameraPosition.x, cameraPosition.y, cameraPosition.z, cameraPosition.w }
	};

	GetContext()->UpdateConstantBuffer(myFrameBuffer.Get(), &frameBuffer, sizeof(frameBuffer));

	GetContext()->SetBlendState(myBlendState.Get());

	GetContext()->SetInputLayout(myInputLayout.Get());
	GetContext()->BindVertexShader(myVertexShader.Get());
	GetContext()->BindGeometryShader(myGeometryShader.Get());
	GetContext()->BindPixelShader(myPixelShader.Get());

	GetContext()->BindTargets(&myRenderTarget, 1, myDepthStencil.Get());
	GetContext()->BindSamplerStates(&mySamplerState, 1, 0, graphics::GRAPHICS_SHADER_STAGE_PIXEL);

	graphics::Viewport viewport = {
		.Left = 0.f,
		.Right = static_cast<float>(myRenderTarget->GetTexture()->GetDesc().Width),
		.Top = 0.f,
		.Bottom = static_cast<float>(myRenderTarget->GetTexture()->GetDesc().Height),
		.Front = 0.f,
		.Back = 1.f
	};
	GetContext()->SetViewports(&viewport, 1);

	std::vector<TextInstance> texts = std::move(myTexts);
	myTexts = {};

	for (size_t i = 0; i < texts.size(); i++)
	{
		const TextInstance& text = texts[i];

		ObjectBuffer objectBuffer = { };
		objectBuffer.ID = 0;
		errno_t error = memcpy_s(&objectBuffer.Transform, sizeof(objectBuffer.Transform), &text.Transform, sizeof(text.Transform));
		if (error != 0)
		{
			//TODO: Log
			throw std::exception("memcpy_s failed!");
		}

		GetContext()->UpdateConstantBuffer(
			myObjectBuffer.Get(),
			&objectBuffer,
			sizeof(objectBuffer)
		);

		GetContext()->BindShaderResources(&text.Text.myAtlas, 1, 0, graphics::GRAPHICS_SHADER_STAGE_PIXEL);

		const size_t stride = sizeof(GlyphVertex);
		GetContext()->BindVertexBuffers(&text.Text.myVertexBuffer, 1, 0, &stride, nullptr);
		GetContext()->Draw(static_cast<unsigned int>(text.Text.myVertexCount), 0u);
	}

	GetContext()->PopEvent();
}
