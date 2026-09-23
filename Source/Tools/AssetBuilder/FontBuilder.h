#pragma once
#include <filesystem>

class FontBuilder
{
public:
	FontBuilder();
	~FontBuilder();

	void SetSignedDistanceLength(float aPxLength) noexcept;
	float GetSignedDistanceLength() const noexcept;

	void SetEmSize(float aPxSize) noexcept;
	float GetEmSize() const noexcept;

	bool Build(const std::filesystem::path& aInputFile, const std::filesystem::path& aOutputFile);
private:

	float mySignedDistanceLength;
	float myEmSize;
};