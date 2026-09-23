#pragma once
#include <filesystem>

class ModelBuilder
{
public:
	ModelBuilder();
	~ModelBuilder();

	bool Build(const std::filesystem::path& aInputFile, const std::filesystem::path& aOutputFile);
private:
};