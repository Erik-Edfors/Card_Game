#include "Font.h"
#include <fstream>
#include <Core/CommonUtilities/Serialization/StreamWrapper.hpp>
#include <Core/CommonUtilities/Serialization/Serializer.hpp>
#include <Core/CommonUtilities/Serialization/Serialization.hpp>

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
    if (outFont == nullptr)
    {
        //TODO: Log
        return false;
    }

    std::ifstream file(aPath, std::ifstream::binary);
    if (not file.is_open())
    {
        //TODO: Log
        return false;
    }

    CommonUtilities::Serializer serializer;
    CommonUtilities::StreamReader reader(file);
    reader.SetConstraints(
        CommonUtilities::ReadConstraints{
            .MaxSize = 1'000'000
        }
    );
    FileHeader header = {};
    if (not serializer.Read(header, reader))
    {
        //TODO: Log
        return false;
    }
    if (header != FONT_FILE_HEADER)
    {
        //TODO: Log
        return false;
    }
    if (not serializer.Read(*outFont, reader))
    {
        //TODO: Log
        return false;
    }

    return true;
}

bool assets::Save(const std::filesystem::path& aPath, const FontFile& aFont)
{
    std::ofstream file(aPath, std::ifstream::binary);
    if (not file.is_open())
    {
        //TODO: Log
        return false;
    }

    CommonUtilities::Serializer serializer;
    CommonUtilities::StreamWriter writer(file);
    writer.SetConstraints(
        CommonUtilities::WriteConstraints{
            .MaxSize = 1'000'000
        }
    );
    if (not serializer.Write(FONT_FILE_HEADER, writer))
    {
        //TODO: Log
        return false;
    }
    if (not serializer.Write(aFont, writer))
    {
        //TODO: Log
        return false;
    }

    return true;
}
