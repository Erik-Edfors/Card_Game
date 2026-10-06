#pragma once
#include <string>
#include <format>
#include <ostream>
#include <Core/CommonUtilities/StringHash.hpp>

#define NETWORK_DEFINE_MESSAGE_ID(ID_) inline static MessageID ID = ID_;

class MessageID
{
public:
	constexpr MessageID() noexcept = default;
	constexpr ~MessageID() noexcept = default;
	constexpr MessageID(const char* aID) noexcept :
		myHash(aID),
		myString(aID)
	{
	}
	constexpr MessageID(std::string_view aID) noexcept :
		myHash(aID),
		myString(aID)
	{
	}
	constexpr MessageID(const MessageID& aMessageID) noexcept :
		myHash(aMessageID.myHash),
		myString(aMessageID.myString)
	{
	}
	constexpr MessageID(MessageID&& aMessageID) noexcept :
		myHash(std::move(aMessageID.myHash)),
		myString(std::move(aMessageID.myString))
	{
		aMessageID = {};
	}

	constexpr MessageID& operator=(const char* aString) noexcept
	{
		myHash = aString;
		myString = aString;
		return *this;
	}
	constexpr MessageID& operator=(std::string_view aView) noexcept
	{
		myHash = aView;
		myString = aView;
		return *this;
	}
	constexpr MessageID& operator=(const MessageID& aMessageID) noexcept
	{
		myHash = aMessageID.myHash;
		myString = aMessageID.myString;
		return *this;
	}
	constexpr MessageID& operator=(MessageID&& aMessageID) noexcept
	{
		myHash = std::move(aMessageID.myHash);
		myString = std::move(aMessageID.myString);
		aMessageID = {};
		return *this;
	}

	constexpr bool operator<(const MessageID& aLHS) const noexcept
	{
		return GetHash() < aLHS.GetHash();
	}
	constexpr bool operator>(const MessageID& aLHS) const noexcept
	{
		return GetHash() > aLHS.GetHash();
	}
	constexpr bool operator==(const MessageID& aLHS) const noexcept
	{
		return GetHash() == aLHS.GetHash() && GetString() == aLHS.GetString();
	}
	constexpr bool operator!=(const MessageID& aLHS) const noexcept
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

std::ostream& operator<<(std::ostream& aStream, const MessageID& aMessageID);

template<>
struct ::std::hash<MessageID>
{
	constexpr hash() = default;
	constexpr ~hash() = default;

	constexpr std::size_t operator()(const MessageID& aMessageID) const noexcept
	{
		return static_cast<size_t>(aMessageID.GetHash().Hash);
	}
};

template<>
struct ::std::formatter<MessageID>
{
	constexpr auto parse(std::format_parse_context& context)
	{
		return std::formatter<std::string>().parse(context);
	}

	auto format(const MessageID& aMessageID, std::format_context& aContext) const
	{
		return std::formatter<std::string>().format(aMessageID.GetString(), aContext);
	}
};