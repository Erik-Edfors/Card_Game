#pragma once
#include "RenderLayer.h"

#include <Core/CommonUtilities/Math/Vector.h>
#include <Core/CommonUtilities/Math/AABB2D.hpp>
#include <Core/CommonUtilities/Math/Matrix4x4.hpp>
#include <Core/RenderHardwareInterface/Objects/Buffers/VertexBuffer.h>

#include <unordered_map>
#include <string>

namespace graphics
{
	class Texture2D;

	class VertexShader;
	class GeometryShader;
	class PixelShader;

	class InputLayout;

	class ConstantBuffer;

	class BlendState;
	class SamplerState;

	class RenderTarget;
	class DepthStencil;
}

class Font
{
	friend class TextRenderLayer;
public:
	struct Glyph
	{
		float Bounds[4];
		float AtlasRect[4];
		float Advance;
	};
	
	Font();
	~Font();

	const Glyph* FindGlyph(wchar_t aChar) const noexcept;

	float GetFontSize() const noexcept;
	float GetAdvanceHeight() const noexcept;
	float GetUnderlineHeight() const noexcept;
	float GetUnderlineThickness() const noexcept;
	float GetStrikethroughHeight() const noexcept;
	float GetStrikethroughThickness() const noexcept;

private:
	graphics::Texture2D* GetAtlas() const noexcept;

	std::unordered_map<wchar_t, Glyph> myGlyphMap;
	graphics::PImpl<graphics::Texture2D> myAtlas;

	float myFontSize;
	float myUnderlineHeight;
	float myUnderlineThickness;
	float myStrikethroughHeight;
	float myStrikethroughThickness;
	float myAdvanceHeight;
};

class Text
{
	friend class TextRenderLayer;
public:
	Text();
	~Text();

	CommonUtilities::Vector2f GetSize() const noexcept;

private:
	void Initialize(graphics::VertexBuffer* aVertexBuffer, size_t aVertexCount, graphics::Texture2D* aAtlas, CommonUtilities::Vector2f aSize);

	CommonUtilities::Vector2f mySize;
	graphics::PImpl<graphics::VertexBuffer> myVertexBuffer;
	size_t myVertexCount;
	graphics::PImpl<graphics::Texture2D> myAtlas;
};

class TextRenderLayer : public RenderLayer
{
public:
	TextRenderLayer();
	~TextRenderLayer();

	Text CreateText(
		const wchar_t* aText,
		const Font& aFont,
		const CommonUtilities::Vector4f& aColor,
		float aRoughness,
		float aMetallicity
	) const;

	void Draw(const Text& aText, const CommonUtilities::Matrix4x4f& aTransform);

	void SetTargets(graphics::RenderTarget* aRenderTarget, graphics::DepthStencil* aDepthStencil);

	void SetView(const CommonUtilities::Matrix4x4f& aView) noexcept;
	void SetProjection(const CommonUtilities::Matrix4x4f& aProjection) noexcept;

protected:
	void OnInit() override;
	void OnRender() override;

private:
	struct TextInstance
	{
		CommonUtilities::Matrix4x4f Transform;
		Text Text;
	};

	std::vector<TextInstance> myTexts;

	CommonUtilities::Matrix4x4f myView;
	CommonUtilities::Matrix4x4f myProjection;

	graphics::PImpl<graphics::VertexShader> myVertexShader;
	graphics::PImpl<graphics::GeometryShader> myGeometryShader;
	graphics::PImpl<graphics::PixelShader> myPixelShader;

	graphics::PImpl<graphics::InputLayout> myInputLayout;

	graphics::PImpl<graphics::ConstantBuffer> myFrameBuffer;
	graphics::PImpl<graphics::ConstantBuffer> myObjectBuffer;
	graphics::PImpl<graphics::ConstantBuffer> myMaterialBuffer;

	graphics::PImpl<graphics::BlendState> myBlendState;

	graphics::PImpl<graphics::RenderTarget> myRenderTarget;
	graphics::PImpl<graphics::DepthStencil> myDepthStencil;
	graphics::PImpl<graphics::SamplerState> mySamplerState;
};