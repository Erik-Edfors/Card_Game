#include "GraphicsEngine.pch.h"
#include "TextRenderLayer.h"

#include <Core/RenderHardwareInterface/Objects/Texture2D.h>
#include <Core/RenderHardwareInterface/Objects/Buffers/VertexBuffer.h>

Font::Font()
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

Text::Text() :
	myColor(1.f, 1.f, 1.f, 1.f),
	myRoughness(1.f),
	myMetallicity(0.f)
{
}

Text::~Text()
{
}

void Text::SetFont(const Font& aFont) noexcept
{
	myFont = aFont;
}

const Font& Text::GetFont() const noexcept
{
	return myFont;
}

void Text::SetText(const wchar_t* aText) noexcept
{
	myText = aText;
}

const wchar_t* Text::GetText() const noexcept
{
	return myText.c_str();
}

void Text::Recalculate() noexcept
{
	std::unique_ptr<GlyphVertex[]> glyphs = std::make_unique<GlyphVertex[]>(myText.length());

	CommonUtilities::Vector2f cursor = { 0.f, 0.f };
	const float originX = 0.f;

	for (size_t i = 0; i < myText.length(); i++)
	{
		const wchar_t c = myText[i];
		const Font::Glyph* glyph = myFont.FindGlyph(c);
		GlyphVertex& vertex = glyphs[i];

		vertex.Position = cursor;
		vertex.TextureRect = { { 0.f, 0.f }, { 0.f, 0.f } };

		if (glyph != nullptr)
		{
			vertex.Position = cursor + CommonUtilities::Vector2f(glyph->Offset[0], glyph->Offset[1]);
			vertex.TextureRect = {
				{ glyph->AtlasRect[0], glyph->AtlasRect[1] },
				{ glyph->AtlasRect[2], glyph->AtlasRect[3] }
			};

			if (c == L'\n')
			{
				cursor.x = originX;
				cursor.y += myFont.GetAdvanceHeight();
			}
			else
			{
				cursor.x += glyph->Advance;
			}
		}
	}
}
