#include <string>

namespace CommonUtilities
{
	std::wstring UTF8(const std::string_view aView);
	std::wstring UTF7(const std::string_view aView);

	std::string ANSI(const std::wstring_view aView);
}