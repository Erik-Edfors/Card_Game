#pragma once
#include <string>
#include <format>
#include <ostream>
#include <Core/CommonUtilities/StringHash.hpp>

namespace assets
{
	class ResourceID
	{
	public:
		constexpr ResourceID() noexcept = default;
		constexpr ~ResourceID() noexcept = default;
		constexpr ResourceID(const char* aID) noexcept :
			myHash(aID),
			myString(aID)
		{
		}
		constexpr ResourceID(std::string_view aID) noexcept :
			myHash(aID),
			myString(aID)
		{
		}
		constexpr ResourceID(const ResourceID& aResourceID) noexcept :
			myHash(aResourceID.myHash),
			myString(aResourceID.myString)
		{
		}
		constexpr ResourceID(ResourceID&& aResourceID) noexcept :
			myHash(std::move(aResourceID.myHash)),
			myString(std::move(aResourceID.myString))
		{
			aResourceID = {};
		}

		constexpr ResourceID& operator=(const ResourceID& aResourceID) noexcept
		{
			myHash = aResourceID.myHash;
			myString = aResourceID.myString;
			return *this;
		}
		constexpr ResourceID& operator=(ResourceID&& aResourceID) noexcept
		{
			myHash = std::move(aResourceID.myHash);
			myString = std::move(aResourceID.myString);
			aResourceID = {};
			return *this;
		}

		constexpr bool operator<(const ResourceID& aLHS) const noexcept
		{
			return GetHash() < aLHS.GetHash();
		}
		constexpr bool operator>(const ResourceID& aLHS) const noexcept
		{
			return GetHash() > aLHS.GetHash();
		}
		constexpr bool operator==(const ResourceID& aLHS) const noexcept
		{
			return GetHash() == aLHS.GetHash() && GetString() == aLHS.GetString();
		}
		constexpr bool operator!=(const ResourceID& aLHS) const noexcept
		{
			return GetHash() != aLHS.GetHash() || GetString() != aLHS.GetString();
		}

		constexpr CommonUtilities::StringHash GetHash() const noexcept
		{
			return myHash;
		}
		constexpr const std::string& GetString() const noexcept
		{
			return myString;
		}

	private:
		CommonUtilities::StringHash myHash;
		std::string myString;
	};
}

std::ostream& operator<<(std::ostream& aStream, const assets::ResourceID& aResourceID);

template<>
struct ::std::hash<assets::ResourceID>
{
	constexpr hash() = default;
	constexpr ~hash() = default;

	constexpr std::size_t operator()(const assets::ResourceID& aResourceID) const noexcept
	{
		return static_cast<size_t>(aResourceID.GetHash().Hash);
	}
};

template<>
struct ::std::formatter<assets::ResourceID>
{
	constexpr auto parse(std::format_parse_context& context)
	{
		return std::formatter<std::string>().parse(context);
	}

	auto format(const assets::ResourceID& aResourceID, std::format_context& aContext) const
	{
		return std::formatter<std::string>().format(aResourceID.GetString(), aContext);
	}
};