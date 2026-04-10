#pragma once
#include <filesystem>
#include <regex>

using FileCallback = void(*)(const std::filesystem::path& aPath, void* aUserData);

void IterateFiles(
	const std::filesystem::path& aPath,
	const std::wregex* aRegex,
	bool aRecursive,
	FileCallback aCallback,
	void* aUserData
);