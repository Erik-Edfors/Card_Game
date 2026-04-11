#pragma once
#include "Common.h"
#include <Core/CommonUtilities/Serialization/BasicTypes.hpp>
#include <Core/CommonUtilities/Serialization/STLtypes.hpp>
#include <Core/CommonUtilities/Serialization/TypeMacros.h>

#include <vector>

namespace assets
{
	static constexpr FileHeader FONT_FILE_HEADER = FileHeader{
		{ 'F', 'O', 'N', 'T' },
		{ 0, 0, 0, 1 }
	};

	struct Font
	{
		struct Glyph
		{
			wchar_t Character;
			float Offset[2];
			float AtlasRect[4];
			float Advance;
		};

		struct Image
		{
			unsigned short Width;
			unsigned short Height;
			std::vector<char> Data;
		};

		unsigned short FontSize;
		float UnderlineHeight;
		float UnderlineThickness;
		float StrikethroughHeight;
		float StrikethroughThickness;
		float AdvanceHeight;

		std::vector<Glyph> Glyphs;
		Image Atlas;
	};

	NBS_DEFINE_TYPE(
		Font::Glyph,
		Character,
		Offset,
		AtlasRect,
		Advance
	);

	NBS_DEFINE_TYPE(
		Font::Image,
		Width,
		Height,
		Data
	);

	NBS_DEFINE_TYPE(
		Font,
		FontSize,
		UnderlineHeight,
		UnderlineThickness,
		StrikethroughHeight,
		StrikethroughThickness,
		AdvanceHeight,
		Glyphs,
		Atlas
	);
}