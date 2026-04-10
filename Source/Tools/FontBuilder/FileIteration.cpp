#include "FileIteration.h"

void IterateFiles(const std::filesystem::path& aPath, const std::wregex* aRegex, bool aRecursive, FileCallback aCallback, void* aUserData)
{
	if (aRecursive)
	{
		std::error_code error;
		std::filesystem::recursive_directory_iterator iterator(aPath, error);
		if (error)
		{
			return;
		}
		for (auto& entry : iterator)
		{
			if (entry.is_regular_file())
			{
				const std::filesystem::path path = entry.path();
				if (aRegex != nullptr)
				{
					if (not std::regex_match(path.c_str(), *aRegex))
					{
						continue;
					}
				}

				(aCallback)(path, aUserData);
			}
		}
	}
	else
	{
		std::error_code error;
		std::filesystem::directory_iterator iterator(aPath, error);
		if (error)
		{
			return;
		}
		for (auto& entry : iterator)
		{
			if (entry.is_regular_file())
			{
				const std::filesystem::path path = entry.path();
				if (aRegex != nullptr)
				{
					if (not std::regex_match(path.c_str(), *aRegex))
					{
						continue;
					}
				}

				(aCallback)(path, aUserData);
			}
		}
	}
}
