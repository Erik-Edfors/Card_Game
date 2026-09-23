#include "ModelBuilder.h"
#include <iostream>

ModelBuilder::ModelBuilder()
{
}

ModelBuilder::~ModelBuilder()
{
}

bool ModelBuilder::Build(const std::filesystem::path& aInputFile, const std::filesystem::path& aOutputFile)
{
	aInputFile;
	aOutputFile;
	std::wcout << L"Failed to build model! Build not yet added." << std::endl;
	return false;
}
