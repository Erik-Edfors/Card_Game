#include "FontBuilder.h"
#include <iostream>
#include <thread>
#pragma warning( push )
#pragma warning( disable : 4505 )	// Ignore 'unreferenced function with internal linkage has been removed' warning.
#pragma warning( disable : 4458 )	// Ignore 'decleration of '?' hides class member' warning.
#include <msdf-atlas-gen.h>
#pragma warning( pop )

#include <Core/Assets/FileFormats/Font.h>
//#include <fstream>

FontBuilder::FontBuilder() : 
	myEmSize(24.f),
	mySignedDistanceLength(4.f)
{
}

FontBuilder::~FontBuilder()
{
}

void FontBuilder::SetSignedDistanceLength(float aPxLength) noexcept
{
	mySignedDistanceLength = aPxLength;
}

float FontBuilder::GetSignedDistanceLength() const noexcept
{
	return mySignedDistanceLength;
}

void FontBuilder::SetEmSize(float aPxSize) noexcept
{
	myEmSize = aPxSize;
}

float FontBuilder::GetEmSize() const noexcept
{
	return myEmSize;
}

bool FontBuilder::Build(const std::filesystem::path& aInputFile, const std::filesystem::path& aOutputFile)
{
	aInputFile;
	aOutputFile;

	msdfgen::FreetypeHandle* ft = msdfgen::initializeFreetype();
	if (ft == nullptr)
	{
		std::wcout << L"Failed to initialize FreeType!" << std::endl;
		return false;
	}

	msdfgen::FontHandle* font = msdfgen::loadFont(ft, aInputFile.string().c_str());
	if (font == nullptr)
	{
		std::wcout << L"Failed to load font!" << std::endl;
		msdfgen::deinitializeFreetype(ft);
		return false;
	}

	std::vector<msdf_atlas::GlyphGeometry> glyphs;

	msdf_atlas::FontGeometry fontGeometry(&glyphs);

	msdf_atlas::Charset ascii;
	for (msdf_atlas::unicode_t cp = 0x0; cp < 0x7f; ++cp)
	{
		ascii.add(cp);
	}

	fontGeometry.loadCharset(font, 1.0, ascii);

	for (msdf_atlas::GlyphGeometry& glyph : glyphs)
	{
		const double maxCornerAngle = 3.0;
		glyph.edgeColoring(&msdfgen::edgeColoringInkTrap, maxCornerAngle, 0);
	}

	msdf_atlas::TightAtlasPacker packer;

	packer.setDimensionsConstraint(msdf_atlas::DimensionsConstraint::SQUARE);
	packer.setMinimumScale(static_cast<double>(myEmSize));
	packer.setPixelRange(static_cast<double>(mySignedDistanceLength));
	packer.setMiterLimit(1.0);
	int packResult = packer.pack(glyphs.data(), static_cast<int>(glyphs.size()));
	if (packResult != 0)
	{
		std::wcout << L"Failed to pack glyph set!" << std::endl;
		msdfgen::destroyFont(font);
		msdfgen::deinitializeFreetype(ft);
		return false;
	}

	int width = 0, height = 0;
	packer.getDimensions(width, height);

	msdf_atlas::ImmediateAtlasGenerator<
		float,
		4,
		msdf_atlas::mtsdfGenerator,
		msdf_atlas::BitmapAtlasStorage<msdf_atlas::byte, 4>
	> generator(width, height);

	msdf_atlas::GeneratorAttributes attributes;
	generator.setAttributes(attributes);
	generator.setThreadCount(static_cast<int>(std::thread::hardware_concurrency()));

	generator.generate(glyphs.data(), static_cast<int>(glyphs.size()));

	msdfgen::BitmapConstSection<msdf_atlas::byte, 4> constSection = generator.atlasStorage();

	const msdfgen::FontMetrics& fontMetrics = fontGeometry.getMetrics();

	assets::FontFile file;
	file.AdvanceHeight = static_cast<float>(fontMetrics.lineHeight);
	file.UnderlineHeight = static_cast<float>(fontMetrics.underlineY);
	file.UnderlineThickness = static_cast<float>(fontMetrics.underlineThickness);

	file.MTSDFAtlas.Width = constSection.width;
	file.MTSDFAtlas.SDLength = mySignedDistanceLength;
	file.MTSDFAtlas.EmSize = myEmSize;
	file.MTSDFAtlas.Pixels.resize(constSection.width * constSection.height);


	for (size_t y = 0; y < constSection.height; y++)
	{
		for (size_t x = 0; x < constSection.width; x++)
		{
			assets::FontFile::Atlas::Pixel& pixel = file.MTSDFAtlas.Pixels[(constSection.height - y - 1) * file.MTSDFAtlas.Width + x];
			const msdf_atlas::byte* atlasPixel = &constSection.pixels[y * constSection.rowStride + x * 4];

			pixel.R = atlasPixel[0];
			pixel.G = atlasPixel[1];
			pixel.B = atlasPixel[2];
			pixel.A = atlasPixel[3];
		}
	}

	for (size_t i = 0; i < glyphs.size(); i++)
	{
		const msdf_atlas::GlyphGeometry& glyphGeometry = glyphs[i];
		const msdf_atlas::GlyphBox& box = glyphGeometry;
		assets::FontFile::Glyph& glyph = file.Glyphs[static_cast<unsigned int>(glyphGeometry.getCodepoint())];

		glyph.WhiteSpace = glyphGeometry.isWhitespace();
		glyph.Advance = static_cast<float>(box.advance);

		// Right -> x, Up -> y.
		glyph.PlaneBounds.Left = static_cast<float>(box.bounds.l);
		glyph.PlaneBounds.Right = static_cast<float>(box.bounds.r);
		glyph.PlaneBounds.Top = static_cast<float>(box.bounds.t);
		glyph.PlaneBounds.Bottom = static_cast<float>(box.bounds.b);

		// Right -> x, Down -> y.
		glyph.AtlasRect.Left = static_cast<unsigned short>(box.rect.x);
		glyph.AtlasRect.Right = static_cast<unsigned short>(box.rect.x + box.rect.w);
		glyph.AtlasRect.Top = static_cast<unsigned short>(height - box.rect.y - box.rect.h);
		glyph.AtlasRect.Bottom = static_cast<unsigned short>(height - box.rect.y);
	}

	if (not Save(aOutputFile, file))
	{
		std::wcout << L"Failed to save file!" << std::endl;
		msdfgen::destroyFont(font);
		msdfgen::deinitializeFreetype(ft);
		return false;
	}

	msdfgen::destroyFont(font);
	msdfgen::deinitializeFreetype(ft);
	return true;
}