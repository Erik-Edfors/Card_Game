#include "Font.h"
#include <Core/Logger/Logger.h>
#include <fstream>
#include <Core/CommonUtilities/Serialization/StreamWrapper.hpp>
#include <Core/CommonUtilities/Serialization/Serializer.hpp>
#include <Core/CommonUtilities/Serialization/Serialization.hpp>

#define FILE_CATEGORY "Font.cpp"

NBS_DEFINE_TYPE(
    assets::FontFile::Atlas,
    Width,
    SDLength,
    EmSize,
    Pixels
);

NBS_DEFINE_TYPE(
    assets::FontFile,
    AdvanceHeight,
    UnderlineHeight,
    UnderlineThickness,
    MTSDFAtlas,
    Glyphs
);

bool assets::Load(const std::filesystem::path& aPath, FontFile* outFont)
{
#define LOG_CATEGORY FILE_CATEGORY"->Load"

    if (outFont == nullptr)
    {
        LOG_ERROR(LOG_CATEGORY, "Failed to load font '%s'! outFont was nullptr.", aPath.string());
        return false;
    }

    std::ifstream file(aPath, std::ifstream::binary);
    if (not file.is_open())
    {
        LOG_ERROR(LOG_CATEGORY, "Failed to load font '%s'! Could not open file.", aPath.string());
        return false;
    }

    CommonUtilities::Serializer serializer;
    CommonUtilities::StreamReader reader(file);

    FileHeader header = {};
    if (not serializer.Read(header, reader, sizeof(header)))
    {
        LOG_ERROR(LOG_CATEGORY, "Failed to load font '%s'! Exception: '%s'.", aPath.string(), serializer.GetLastException().what());
        return false;
    }
    if (header != FONT_FILE_HEADER)
    {
        LOG_ERROR(LOG_CATEGORY, "Failed to load font '%s'! Version mismatch, Expected: '%s, %i.%i.%i.%i', Actual '%s, %i.%i.%i.%i'", 
            aPath.string(),
            
            FONT_FILE_HEADER.Identifier,
            FONT_FILE_HEADER.Version[0],
            FONT_FILE_HEADER.Version[1],
            FONT_FILE_HEADER.Version[2],
            FONT_FILE_HEADER.Version[3],

            header.Identifier,
            header.Version[0],
            header.Version[1],
            header.Version[2],
            header.Version[3]
        );
        return false;
    }
    if (not serializer.Read(*outFont, reader, 10'000'000))
    {
        LOG_ERROR(LOG_CATEGORY, "Failed to load font '%s'! Exception: '%s'.", aPath.string(), serializer.GetLastException().what());
        return false;
    }

    return true;

#undef LOG_CATEGORY
}

bool assets::Save(const std::filesystem::path& aPath, const FontFile& aFont)
{
#define LOG_CATEGORY FILE_CATEGORY"->Save"

    std::ofstream file(aPath, std::ifstream::binary);
    if (not file.is_open())
    {
        LOG_ERROR(LOG_CATEGORY, "Failed to save font to '%s'! Could not open file.", aPath.string());
        return false;
    }

    CommonUtilities::Serializer serializer;
    CommonUtilities::StreamWriter writer(file);

    if (not serializer.Write(FONT_FILE_HEADER, writer))
    {
        LOG_ERROR(LOG_CATEGORY, "Failed to save font to '%s'! Exception: '%s'.", aPath.string(), serializer.GetLastException().what());
        return false;
    }
    if (not serializer.Write(aFont, writer))
    {
        LOG_ERROR(LOG_CATEGORY, "Failed to save font to '%s'! Exception: '%s'.", aPath.string(), serializer.GetLastException().what());
        return false;
    }

    return true;

#undef LOG_CATEGORY
}

#undef FILE_CATEGORY