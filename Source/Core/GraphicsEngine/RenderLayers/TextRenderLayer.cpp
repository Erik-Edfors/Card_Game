#include "GraphicsEngine.pch.h"
#include "TextRenderLayer.h"

#include <Core/CommonUtilities/Math/Vector2.hpp>
#include <Core/CommonUtilities/StringHelper.h>

#include <Core/Assets/ShaderRegistry.h>

#include <Core/RenderHardwareInterface/Device.h>
#include <Core/RenderHardwareInterface/Objects/DeferredDeviceContext.h>
#include <Core/RenderHardwareInterface/Objects/Texture2D.h>
#include <Core/RenderHardwareInterface/Objects/ShaderResource.h>
#include <Core/RenderHardwareInterface/Objects/RenderTarget.h>
#include <Core/RenderHardwareInterface/Objects/DepthStencil.h>
#include <Core/RenderHardwareInterface/Objects/SamplerState.h>
#include <Core/RenderHardwareInterface/Objects/Buffers/ConstantBuffer.h>
#include <Core/RenderHardwareInterface/Objects/Buffers/VertexBuffer.h>
#include <Core/RenderHardwareInterface/Objects/BlendState.h>
#include <Core/RenderHardwareInterface/Objects/InputLayout.h>
#include <Core/RenderHardwareInterface/Objects/Shaders/VertexShader.h>
#include <Core/RenderHardwareInterface/Objects/Shaders/GeometryShader.h>
#include <Core/RenderHardwareInterface/Objects/Shaders/PixelShader.h>

struct FrameBuffer
{
	CommonUtilities::Matrix4x4f View;
	CommonUtilities::Matrix4x4f Projection;
};
struct ObjectBuffer
{
	CommonUtilities::Matrix4x4f Transform;
	CommonUtilities::Vector4f Color;
	CommonUtilities::Vector4f BorderColor;
	float AtlasEmSize;
	float SDLength;
	float BorderThickness;

	unsigned int _padding[1];
};

Text::Text() noexcept :
	myFont(nullptr),
	myObjectBufferIsDirty(true),
	myVerticesIsDirty(true),
	myAtlasIsDirty(true),
	myInstance({}),
	myColor({ 0.f, 0.f, 0.f, 1.f }),
	myBorderColor({ 1.f, 1.f, 1.f, 1.f }),
	myBorderThickness(0.f),
	myAdvancePadding(0.f),
	myFontSize(24),
	myMaxWidth(INFINITY)
{
}

Text::~Text() noexcept
{
}

void Text::SetTransform(const CommonUtilities::Matrix4x4f& aTransform) noexcept
{
	myObjectBufferIsDirty |= myTransform != aTransform;
	myTransform = aTransform;
}

const CommonUtilities::Matrix4x4f& Text::GetTransform() const noexcept
{
	return myTransform;
}

void Text::SetColor(CommonUtilities::Vector4f aColor) noexcept
{
	myObjectBufferIsDirty |= true;
	myColor = aColor;
}

CommonUtilities::Vector4f Text::GetColor() const noexcept
{
	return myColor;
}

void Text::SetBorderColor(CommonUtilities::Vector4f aColor) noexcept
{
	myObjectBufferIsDirty |= true;
	myBorderColor = aColor;
}

CommonUtilities::Vector4f Text::GetBorderColor() const noexcept
{
	return myBorderColor;
}

void Text::SetBorderThickness(float aBorderThickness) noexcept
{
	myObjectBufferIsDirty |= true;
	myBorderThickness = aBorderThickness;
}

float Text::GetBorderThickness() const noexcept
{
	return myBorderThickness;
}

void Text::SetAdvancePadding(float aPadding) noexcept
{
	myVerticesIsDirty |= true;
	myAdvancePadding = aPadding;
}

float Text::GetAdvancePadding() const noexcept
{
	return myAdvancePadding;
}

void Text::SetFont(const assets::FontFile* aFont) noexcept
{
	myAtlasIsDirty |= true;
	myVerticesIsDirty |= true;
	myFont = aFont;
}

const assets::FontFile* Text::GetFont() const noexcept
{
	return myFont;
}

void Text::SetFontSize(float aFontSize) noexcept
{
	myObjectBufferIsDirty |= true;
	myFontSize = aFontSize;
}

float Text::GetFontSize() const noexcept
{
	return myFontSize;
}

void Text::SetMaxWidth(float aMaxWidth) noexcept
{
	myVerticesIsDirty |= true;
	myMaxWidth = aMaxWidth;
}

float Text::GetMaxWidth() const noexcept
{
	return myMaxWidth;
}

void Text::SetText(const wchar_t* aText) noexcept
{
	myVerticesIsDirty |= true;
	myText = aText;
}

void Text::SetText(const char* aText) noexcept
{
	myVerticesIsDirty |= true;
	myText = CommonUtilities::ToWide(aText, CommonUtilities::STRING_ENCODING_UTF8);
}

std::wstring_view Text::GetText() const noexcept
{
	return myText;
}

Text::TextInstance Text::GetInstance(graphics::Device* aDevice) const
{
	Clean(aDevice);
	return myInstance;
}

void Text::Clean(graphics::Device* aDevice) const
{
	if (myObjectBufferIsDirty)
	{
		float emSize = 24.f;
		float sdLength = 4.f;

		if (myFont != nullptr)
		{
			emSize = myFont->MTSDFAtlas.EmSize;
			sdLength = myFont->MTSDFAtlas.SDLength;
		}

		const ObjectBuffer buffer = {
			.Transform = myTransform,
			.Color = myColor,
			.BorderColor = myBorderColor,
			.AtlasEmSize = emSize,
			.SDLength = sdLength,
			.BorderThickness = myBorderThickness,
			._padding = {}
		};

		const graphics::ConstantBufferDesc desc = {
			.Name = "Text ObjectBuffer",
			.InitialData = &buffer,
			.Size = sizeof(buffer),
			.Usage = graphics::GRAPHICS_USAGE_IMMUTABLE,
			.CPUAccessFlags = graphics::GRAPHICS_CPU_ACCESS_NONE
		};

		graphics::result::Code result = aDevice->CreateConstantBuffer(&myInstance.ObjectBuffer, &desc);
		result;
		//TODO: Log errors.

		myObjectBufferIsDirty = false;
	}
	if (myAtlasIsDirty)
	{
		myInstance.Atlas.Reset();

		if (myFont != nullptr)
		{
			const assets::FontFile::Atlas& atlas = myFont->MTSDFAtlas;

			graphics::PImpl<graphics::Texture2D> atlasTexture;
			{
				graphics::TextureSubresourceData data = {
					.Data = atlas.Pixels.data(),
					.RowPitch = sizeof(atlas.Pixels[0]) * atlas.Width,
					.DepthPitch = static_cast<unsigned int>(sizeof(atlas.Pixels[0]) * atlas.Pixels.size())
				};
				graphics::Texture2DDesc desc = {
					.BindFlags = graphics::GRAPHICS_BIND_SHADER_RESOURCE,
					.ResourceFlags = graphics::GRAPHICS_RESOURCE_NONE,
					.Name = "Text Atlas Texture",
					.Width = atlas.Width,
					.Height = static_cast<unsigned int>(atlas.Pixels.size()) / atlas.Width,
					.MipLevels = 1,
					.NumTextures = 1,
					.Data = &data,
					.NumData = 1,
					.Format = graphics::GRAPHICS_FORMAT_R8G8B8A8_UNORM,
					.Usage = graphics::GRAPHICS_USAGE_IMMUTABLE,
					.CPUAccessFlags = graphics::GRAPHICS_CPU_ACCESS_NONE
				};
				graphics::result::Code result = aDevice->CreateTexture2D(&atlasTexture, &desc);
				result;
				//TODO: Log errors.
			}
			{
				graphics::ShaderResourceDesc desc = {
					.Name = "Text Atlas ShaderResource",
					.Texture = atlasTexture.Get(),
					.Format = graphics::GRAPHICS_FORMAT_R8G8B8A8_UNORM,
					.ViewDimension = graphics::GRAPHICS_SHADER_RESOURCE_VIEW_DIMENSION_TEXTURE2D,
					.FirstSlice = 0,
					.NumSlices = 1,
					.MipLevels = 1,
					.MostDetailedMip = 0
				};
				graphics::result::Code result = aDevice->CreateShaderResource(&myInstance.Atlas, &desc);
				result;
				//TODO: Log errors.
			}
		}

		myAtlasIsDirty = false;
	}
	if (myVerticesIsDirty)
	{
		myInstance.VertexBuffer.Reset();
		if (not myText.empty() && myFont != nullptr)
		{
			//const float atlasEmSize = 24;	// Number of pixels per Em distance in atlas.
			//const float atlasDistanceFactor = 6;	

			const float atlasWidth = static_cast<float>(myFont->MTSDFAtlas.Width);
			const float atlasHeight = static_cast<float>(myFont->MTSDFAtlas.Pixels.size()) / atlasWidth;

			//const float advancePadding = myBorderThickness * myFontSize;
			const float advancePadding = myAdvancePadding * myFontSize;
			const float advanceHeight = myFont->AdvanceHeight * myFontSize + advancePadding;

			std::vector<GlyphVertex> vertices;
			std::vector<GlyphVertex> wordVertices;
			std::vector<wchar_t> wordCharacters;
			CommonUtilities::Vector2f cursor;
			const wchar_t* current = myText.c_str();
			while (*current != '\0')
			{
				wchar_t c = *current++;
				auto it = myFont->Glyphs.find(c);

				const CommonUtilities::Vector2f currentCursor = cursor;

				if (not wordVertices.empty())
				{
					GlyphVertex& lastVertex = wordVertices.back();

					const float wordRight = lastVertex.Bounds[1];
					if (wordRight > myMaxWidth)
					{
						const GlyphVertex& firstVertex = wordVertices.front();
						const wchar_t firstCharacter = wordCharacters.front();
						const assets::FontFile::Glyph& firstGlyph = myFont->Glyphs.at(firstCharacter);

						const float wordOrigin = firstVertex.Bounds[0] - myFontSize * firstGlyph.PlaneBounds.Left;
						const float wordAdvance = currentCursor.x - wordOrigin;

						if (wordOrigin != 0.f)	// If the word is not already at the start of a new line
						{
							for (GlyphVertex& vertex : wordVertices)
							{
								vertex.Bounds[0] -= wordOrigin;
								vertex.Bounds[1] -= wordOrigin;
								vertex.Bounds[2] -= advanceHeight;
								vertex.Bounds[3] -= advanceHeight;
							}

							cursor.y -= advanceHeight;
							cursor.x = wordAdvance;
						}
					}
				}

				if (c == L'\n')
				{
					cursor.y -= advanceHeight;
					cursor.x = 0.f;
				}

				if (it != myFont->Glyphs.end())
				{
					const assets::FontFile::Glyph& glyph = it->second;

					if (not glyph.WhiteSpace)
					{
						GlyphVertex vertex = {
							.Bounds = {
								cursor.x + myFontSize * glyph.PlaneBounds.Left,
								cursor.x + myFontSize * glyph.PlaneBounds.Right,
								cursor.y + myFontSize * glyph.PlaneBounds.Top,
								cursor.y + myFontSize * glyph.PlaneBounds.Bottom
							},
							.UVRect = {
								(static_cast<float>(glyph.AtlasRect.Left) + 0.5f) / atlasWidth,		// 0.5 is added as otherwise there can be bleedover from other glyphs.
								(static_cast<float>(glyph.AtlasRect.Right) - 0.5f) / atlasWidth,
								(static_cast<float>(glyph.AtlasRect.Top) + 0.5f) / atlasHeight,
								(static_cast<float>(glyph.AtlasRect.Bottom) - 0.5f) / atlasHeight
							}
						};
						wordVertices.push_back(vertex);
						wordCharacters.push_back(c);
					}
					else
					{
						vertices.insert(vertices.end(), wordVertices.begin(), wordVertices.end());
						wordVertices.clear();
						wordCharacters.clear();
					}

					cursor.x += glyph.Advance * myFontSize + advancePadding;
				}
			}

			vertices.insert(vertices.end(), wordVertices.begin(), wordVertices.end());
			wordVertices.clear();
			wordCharacters.clear();

			myInstance.VertexCount = static_cast<unsigned int>(vertices.size());

			graphics::VertexBufferDesc desc = {
				.Name = "Text VertexBuffer",
				.Data = vertices.data(),
				.Size = vertices.size() * sizeof(vertices[0]),
				.Usage = graphics::GRAPHICS_USAGE_IMMUTABLE,
				.CPUAccessFlags = graphics::GRAPHICS_CPU_ACCESS_NONE
			};

			graphics::result::Code result = aDevice->CreateVertexBuffer(&myInstance.VertexBuffer, &desc);
			result;
			//TODO: Log errors.
		}

		myVerticesIsDirty = false;
	}
}

TextRenderLayer::TextRenderLayer(assets::ShaderRegistry* aShaderRegistry) :
	myShaderRegistry(aShaderRegistry)
{
}

void TextRenderLayer::SetRenderTarget(graphics::RenderTarget* aRenderTarget)
{
	myRenderTarget = aRenderTarget;
}

void TextRenderLayer::SetDepthStencil(graphics::DepthStencil* aDepthStencil)
{
	myDepthStencil = aDepthStencil;
}

void TextRenderLayer::SetView(const CommonUtilities::Matrix4x4<float>& aView)
{
	myView = aView;
}

void TextRenderLayer::SetProjection(const CommonUtilities::Matrix4x4<float>& aProjection)
{
	myProjection = aProjection;
}

void TextRenderLayer::Draw(const Text& aText)
{
	myInstances.push_back(aText.GetInstance(GetDevice()));
}

//Text TextRenderLayer::CreateText(const char* aText, const assets::FontFile& aFont, float aSize, const CommonUtilities::Matrix4x4f& aTransform) const
//{
//	Text text;
//
//	text.SetFont(aFont);
//	text.SetFontSize(24);
//
//	graphics::result::Code result;
//	TextInstance instance = {};
//
//	float atlasWidth = static_cast<float>(aFont.MTSDFAtlas.Width);
//	float atlasHeight = static_cast<float>(aFont.MTSDFAtlas.Pixels.size()) / atlasWidth;
//
//	graphics::PImpl<graphics::Texture2D> atlasTexture;
//	{
//		graphics::TextureSubresourceData data = {
//			.Data = aFont.MTSDFAtlas.Pixels.data(),
//			.RowPitch = sizeof(aFont.MTSDFAtlas.Pixels[0]) * aFont.MTSDFAtlas.Width,
//			.DepthPitch = static_cast<unsigned int>(sizeof(aFont.MTSDFAtlas.Pixels[0]) * aFont.MTSDFAtlas.Pixels.size())
//		};
//		graphics::Texture2DDesc desc = {
//			.BindFlags = graphics::GRAPHICS_BIND_SHADER_RESOURCE,
//			.ResourceFlags = graphics::GRAPHICS_RESOURCE_NONE,
//			.Name = "Text Atlas Texture",
//			.Width = aFont.MTSDFAtlas.Width,
//			.Height = static_cast<unsigned int>(aFont.MTSDFAtlas.Pixels.size()) / aFont.MTSDFAtlas.Width,
//			.MipLevels = 1,
//			.NumTextures = 1,
//			.Data = &data,
//			.NumData = 1,
//			.Format = graphics::GRAPHICS_FORMAT_R8G8B8A8_UNORM,
//			.Usage = graphics::GRAPHICS_USAGE_IMMUTABLE,
//			.CPUAccessFlags = graphics::GRAPHICS_CPU_ACCESS_NONE
//		};
//		result = GetDevice()->CreateTexture2D(&atlasTexture, &desc);
//	}
//	{
//		graphics::ShaderResourceDesc desc = {
//			.Name = "Text Atlas ShaderResource",
//			.Texture = atlasTexture.Get(),
//			.Format = graphics::GRAPHICS_FORMAT_R8G8B8A8_UNORM,
//			.ViewDimension = graphics::GRAPHICS_SHADER_RESOURCE_VIEW_DIMENSION_TEXTURE2D,
//			.FirstSlice = 0,
//			.NumSlices = 1,
//			.MipLevels = 1,
//			.MostDetailedMip = 0
//		};
//		result = GetDevice()->CreateShaderResource(&instance.Atlas, &desc);
//	}
//	{
//		std::vector<TextInstance::GlyphVertex> vertices;
//		CommonUtilities::Vector2f cursor;
//		const char* current = aText;
//		while (*current != '\0')
//		{
//			char c = *current++;
//			auto it = aFont.Glyphs.find(c);
//
//			if (c == '\n')
//			{
//				cursor.y -= aFont.AdvanceHeight * aSize;
//				cursor.x = 0.f;
//				continue;
//			}
//
//			if (it != aFont.Glyphs.end())
//			{
//				const assets::FontFile::Glyph& glyph = it->second;
//
//				if (not glyph.WhiteSpace)
//				{
//					TextInstance::GlyphVertex vertex = {
//						.Bounds = {
//							cursor.x + aSize * glyph.PlaneBounds.Left,
//							cursor.x + aSize * glyph.PlaneBounds.Right,
//							cursor.y + aSize * glyph.PlaneBounds.Top,
//							cursor.y + aSize * glyph.PlaneBounds.Bottom
//						},
//						.UVRect = {
//							static_cast<float>(glyph.AtlasRect.Left) / atlasWidth,
//							static_cast<float>(glyph.AtlasRect.Right) / atlasWidth,
//							static_cast<float>(glyph.AtlasRect.Top) / atlasHeight,
//							static_cast<float>(glyph.AtlasRect.Bottom) / atlasHeight
//						}
//					};
//					vertices.push_back(vertex);
//				}
//
//				cursor.x += glyph.Advance * aSize;
//			}
//		}
//
//		instance.VertexCount = static_cast<unsigned int>(vertices.size());
//
//		graphics::VertexBufferDesc desc = {
//			.Name = "Text VertexBuffer",
//			.Data = vertices.data(),
//			.Size = vertices.size() * sizeof(vertices[0]),
//			.Usage = graphics::GRAPHICS_USAGE_IMMUTABLE,
//			.CPUAccessFlags = graphics::GRAPHICS_CPU_ACCESS_NONE
//		};
//		result = GetDevice()->CreateVertexBuffer(&instance.VertexBuffer, &desc);
//	}
//	{
//		graphics::ConstantBufferDesc desc = {
//			.Name = "Text ObjectBuffer",
//			.InitialData = &aTransform,
//			.Size = sizeof(aTransform),
//			.Usage = graphics::GRAPHICS_USAGE_DYNAMIC,
//			.CPUAccessFlags = graphics::GRAPHICS_CPU_ACCESS_WRITE
//		};
//		result = GetDevice()->CreateConstantBuffer(&instance.ObjectBuffer, &desc);
//	}
//
//	return instance;
//}

void TextRenderLayer::OnInit()
{
	const assets::ShaderAsset* sh;
	sh = myShaderRegistry->Get("core:text_vs");

	{
		graphics::InputLayoutDesc desc = {
			.Name = "Text InputLayout",
			.LayoutDefinition = {
				{ "BOUNDS", 0u, graphics::GRAPHICS_FORMAT_R32G32B32A32_FLOAT },
				{ "UVRECT", 0u, graphics::GRAPHICS_FORMAT_R32G32B32A32_FLOAT },
			},
			.ShaderByteCode = sh->ByteCode.get(),
			.NumBytes = sh->ByteCodeSize
		};
		GetDevice()->CreateInputLayout(&myInputLayout, &desc);
	}
	{
		graphics::BlendStateDesc desc = {
			.Name = "Text BlendState",
			.EnableAlphaToCoverage = false,
			.EnableIndependentBlend = false,
			.RenderTargetBlendDescriptions = {
				{
					.EnableBlend = true,
					.SourceBlend = graphics::GRAPHICS_BLEND_SRC_ALPHA,
					.DestinationBlend = graphics::GRAPHICS_BLEND_INV_SRC_ALPHA,
					.BlendOperation = graphics::GRAPHICS_BLEND_OPERATION_ADD,
					.AlphaSourceBlend = graphics::GRAPHICS_BLEND_ONE,
					.AlphaDestinationBlend = graphics::GRAPHICS_BLEND_ZERO,
					.AlphaBlendOperation = graphics::GRAPHICS_BLEND_OPERATION_ADD,
					.RenderTargetWriteMask = graphics::GRAPHICS_BLEND_COLOR_WRITE_ALL
				}
			}
		};
		GetDevice()->CreateBlendState(&myBlendState, &desc);
	}

	if (sh != nullptr)
	{
		sh->Shader->As(&myVertexShader);
	}
	sh = myShaderRegistry->Get("core:text_gs");
	if (sh != nullptr)
	{
		sh->Shader->As(&myGeometryShader);
	}
	sh = myShaderRegistry->Get("core:text_ps");
	if (sh != nullptr)
	{
		sh->Shader->As(&myPixelShader);
	}

	{
		graphics::ConstantBufferDesc desc = {
			.Name = "Frame Buffer",
			.InitialData = nullptr,
			.Size = sizeof(FrameBuffer),
			.Usage = graphics::GRAPHICS_USAGE_DYNAMIC,
			.CPUAccessFlags = graphics::GRAPHICS_CPU_ACCESS_WRITE
		};
		graphics::result::Code result = GetDevice()->CreateConstantBuffer(&myFrameBuffer, &desc);
		if (GRAPHICS_FAILED(result))
		{
			//TODO: LOG
		}
	}

	{
		graphics::SamplerStateDesc desc = {
			.Name = "Trilinear Sampler",
			.Filter = graphics::GRAPHICS_SAMPLER_FILTER_MIN_MAG_MIP_LINEAR,
			.AddressU = graphics::GRAPHICS_TEXTURE_ADDRESS_WRAP,
			.AddressV = graphics::GRAPHICS_TEXTURE_ADDRESS_WRAP,
			.AddressW = graphics::GRAPHICS_TEXTURE_ADDRESS_WRAP,
			.MipLODBias = 0.f,
			.MaxAnistropy = 0,
			.Comparison = graphics::GRAPHICS_COMPARISON_NEVER,
			.BorderColor = { 0.f, 0.f, 0.f, 0.f },
			.MinLOD = 0.f,
			.MaxLOD = FLT_MAX
		};

		graphics::result::Code result = GetDevice()->CreateSamplerState(&mySampler, &desc);
		if (GRAPHICS_FAILED(result))
		{
			//TODO: LOG
		}
	}
}

void TextRenderLayer::OnRender()
{
	if (not myRenderTarget)
	{
		return;
	}
	if (myInstances.empty())
	{
		return;
	}

	graphics::result::Code result;

	GetContext()->PushEvent("Text Rendering");

	//float clearColor[4] = { 0.f, 0.f, 0.f, 1.f };
	//result = GetContext()->ClearRenderTarget(myRenderTarget.Get(), clearColor);

	//TODO: Set InputLayout

	FrameBuffer frameBuffer = {
		.View = myView,
		.Projection = myProjection
	};
	result = GetContext()->UpdateConstantBuffer(myFrameBuffer.Get(), &frameBuffer, sizeof(frameBuffer));
	result = GetContext()->SetInputLayout(myInputLayout.Get());
	result = GetContext()->SetBlendState(myBlendState.Get());
	result = GetContext()->BindConstantBuffers(&myFrameBuffer, 1, 0, graphics::GRAPHICS_SHADER_STAGE_VERTEX);

	result = GetContext()->BindSamplerStates(&mySampler, 1, 0, graphics::GRAPHICS_SHADER_STAGE_PIXEL);

	result = GetContext()->BindVertexShader(myVertexShader.Get());
	result = GetContext()->BindGeometryShader(myGeometryShader.Get());
	result = GetContext()->BindPixelShader(myPixelShader.Get());

	result = GetContext()->SetTopology(graphics::GRAPHICS_TOPOLOGY_POINTLIST);

	const graphics::Texture2DDesc& desc = myRenderTarget->GetDesc().Texture->GetDesc();

	const graphics::Viewport viewport = {
		.Left = 0.f,
		.Right = static_cast<float>(desc.Width),
		.Top = 0.f,
		.Bottom = static_cast<float>(desc.Height),
		.Front = 0.f,
		.Back = 1.f
	};

	result = GetContext()->SetViewports(&viewport, 1);
	result = GetContext()->BindTargets(&myRenderTarget, 1, myDepthStencil.Get());

	for (const Text::TextInstance& instance : myInstances)
	{
		result = GetContext()->BindShaderResources(&instance.Atlas, 1, 0, graphics::GRAPHICS_SHADER_STAGE_PIXEL);
		constexpr size_t stride = sizeof(Text::GlyphVertex);
		result = GetContext()->BindVertexBuffers(&instance.VertexBuffer, 1, 0, &stride, nullptr);
		result = GetContext()->BindConstantBuffers(&instance.ObjectBuffer, 1, 1, graphics::GRAPHICS_SHADER_STAGE_VERTEX | graphics::GRAPHICS_SHADER_STAGE_PIXEL);

		GetContext()->Draw(instance.VertexCount, 0);
	}

	GetContext()->PopEvent();
}

void TextRenderLayer::OnPostRender()
{
	myInstances.clear();
}
