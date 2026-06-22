#pragma once
#include "Common.h"
#include <Core/CommonUtilities/Serialization/BasicTypes.hpp>
#include <Core/CommonUtilities/Serialization/STLtypes.hpp>
#include <Core/CommonUtilities/Serialization/TypeMacros.h>

#include <vector>

namespace assets
{
	static constexpr FileHeader FONT_FILE_HEADER = FileHeader{
		.Type = { 'F', 'O', 'N', 'T' },
		.Version = { 0, 0, 0, 6 }
	};

	struct Font
	{
		struct Glyph
		{
			wchar_t Character;
			float Bounds[4];
			float AtlasRect[4];
			float Advance;
		};

		struct Image
		{
			unsigned short Width;
			unsigned short Height;
			unsigned int RowPitch;
			unsigned int DepthPitch;
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
		Bounds,
		AtlasRect,
		Advance
	);

	NBS_DEFINE_TYPE(
		Font::Image,
		Width,
		Height,
		RowPitch,
		DepthPitch,
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