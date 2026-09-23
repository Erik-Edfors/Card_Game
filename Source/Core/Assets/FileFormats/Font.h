#pragma once
#include <vector>
#include <unordered_map>
#include <filesystem>
#include "Common.h"

namespace assets
{
	constexpr FileHeader FONT_FILE_HEADER = {
		.Identifier = { 'F', 'O', 'N', 'T' },
		.Version = { 0, 0, 0, 2 }
	};

	struct FontFile
	{
		struct Atlas
		{
			struct Pixel
			{
				unsigned char R;
				unsigned char G;
				unsigned char B;
				unsigned char A;
			};

			unsigned int Width;
			float SDLength;
			float EmSize;
			std::vector<Pixel> Pixels;
		};

		struct Glyph
		{
			struct Rect
			{
				unsigned short Left, Right, Top, Bottom;
			};

			struct Bounds
			{
				float Left, Right, Top, Bottom;
			};
			
			Rect AtlasRect;
			Bounds PlaneBounds;
			float Advance;
			bool WhiteSpace;
		};

		float AdvanceHeight;
		float UnderlineHeight;
		float UnderlineThickness;
		Atlas MTSDFAtlas;
		std::unordered_map<unsigned int, Glyph> Glyphs;
	};

	bool Load(const std::filesystem::path& aPath, FontFile* outFont);
	bool Save(const std::filesystem::path& aPath, const FontFile& aFont);
}