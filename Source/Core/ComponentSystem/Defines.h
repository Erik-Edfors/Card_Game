#pragma once
#include <limits>

namespace std
{
	template <class _Kty>
	struct hash;
}

namespace cs
{
	struct EntityHandle
	{
		unsigned int Handle;
	};
	struct ComponentHandle
	{
		unsigned int Handle;
	};

	constexpr EntityHandle INVALID_ENTITY_HANDLE = cs::EntityHandle{ .Handle = 0 };
	constexpr ComponentHandle INVALID_COMPONENT_HANDLE = cs::ComponentHandle{ .Handle = 0 };

	inline constexpr bool operator ==(::cs::EntityHandle aLHS, ::cs::EntityHandle aRHS) noexcept { return aLHS.Handle == aRHS.Handle; }
	inline constexpr bool operator !=(::cs::EntityHandle aLHS, ::cs::EntityHandle aRHS) noexcept { return aLHS.Handle != aRHS.Handle; }
	inline constexpr bool operator <(::cs::EntityHandle aLHS, ::cs::EntityHandle aRHS) noexcept { return aLHS.Handle < aRHS.Handle; }
	inline constexpr bool operator >(::cs::EntityHandle aLHS, ::cs::EntityHandle aRHS) noexcept { return aLHS.Handle > aRHS.Handle; }

	inline constexpr bool operator ==(::cs::ComponentHandle aLHS, ::cs::ComponentHandle aRHS) noexcept { return aLHS.Handle == aRHS.Handle; }
	inline constexpr bool operator !=(::cs::ComponentHandle aLHS, ::cs::ComponentHandle aRHS) noexcept { return aLHS.Handle != aRHS.Handle; }
	inline constexpr bool operator <(::cs::ComponentHandle aLHS, ::cs::ComponentHandle aRHS) noexcept { return aLHS.Handle < aRHS.Handle; }
	inline constexpr bool operator >(::cs::ComponentHandle aLHS, ::cs::ComponentHandle aRHS) noexcept { return aLHS.Handle > aRHS.Handle; }
}

template<>
struct ::std::hash<::cs::EntityHandle>
{
	constexpr hash() = default;
	constexpr ~hash() = default;

	constexpr size_t operator()(const ::cs::EntityHandle& aHandle) const noexcept
	{
		return static_cast<size_t>(aHandle.Handle);
	}
};

template<>
struct ::std::hash<::cs::ComponentHandle>
{
	constexpr hash() = default;
	constexpr ~hash() = default;

	constexpr size_t operator()(const ::cs::ComponentHandle& aHandle) const noexcept
	{
		return static_cast<size_t>(aHandle.Handle);
	}
};