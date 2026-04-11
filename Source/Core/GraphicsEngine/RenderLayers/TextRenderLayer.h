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
}

struct GlyphVertex
{
	CommonUtilities::Vector2f Position;
	CommonUtilities::AABB2Df TextureRect;
};

class Font
{
public:
	struct Glyph
	{
		float AtlasRect[4];
		float Offset[2];
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
public:
	Text();
	~Text();

	void SetFont(const Font& aFont) noexcept;
	const Font& GetFont() const noexcept;

	void SetText(const wchar_t* aText) noexcept;
	const wchar_t* GetText() const noexcept;

private:
	void Recalculate() noexcept;

	Font myFont;
	std::wstring myText;

	CommonUtilities::Matrix4x4f myTransform;
	CommonUtilities::Vector4f myColor;
	float myRoughness;
	float myMetallicity;

	graphics::PImpl<graphics::VertexBuffer> myVertexBuffer;
};

class TextRenderLayer
{
public:

private:
};