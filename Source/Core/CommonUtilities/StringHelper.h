#include <string>

namespace CommonUtilities
{
	enum : unsigned short
	{
		STRING_ENCODING_ANSI,
		STRING_ENCODING_UTF8,
		STRING_ENCODING_UTF7
	};
	using STRING_ENCODING = unsigned short;

	std::wstring ToWide(const std::string_view aView, STRING_ENCODING aEncoding);
	std::string ToNarrow(const std::wstring_view aView, STRING_ENCODING aEncoding);
}