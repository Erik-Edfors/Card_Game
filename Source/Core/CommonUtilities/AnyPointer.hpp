#pragma once
#include <typeinfo>
#include <type_traits>

namespace CommonUtilities
{
	class AnyPointer
	{
	public:
		AnyPointer() noexcept;
		AnyPointer(nullptr_t) noexcept;
		//AnyPointer(const AnyPointer&) = default;
		//AnyPointer(AnyPointer&&) = default;

		template<class _Ty>
			requires(not std::is_same_v<_Ty, void>)
		AnyPointer(_Ty* aPtr) noexcept;

		template<class _Ty>
		_Ty* TryGet() const noexcept;

		void* Get() const noexcept;
		const std::type_info* GetTypeInfo() const noexcept;
		bool IsNull() const noexcept;

		template<class _Ty>
		explicit operator _Ty* () const noexcept;

		operator bool() const noexcept;

	private:
		const std::type_info* myTypeInfo;
		void* myPtr;
	};

	inline AnyPointer::AnyPointer() noexcept :
		myTypeInfo(nullptr),
		myPtr(nullptr)
	{
	}
	inline AnyPointer::AnyPointer(nullptr_t) noexcept :
		myTypeInfo(nullptr),
		myPtr(nullptr)
	{
	}
	//inline AnyPointer::AnyPointer(void*) noexcept : 
	//	myPtr(nullptr),
	//	myTypeInfo(nullptr)
	//{
	//	static_assert("Is is not allowed to assign AnyPointer with void*!");
	//}
	//inline AnyPointer::AnyPointer(const AnyPointer& aAnyPtr) noexcept :
	//	myTypeInfo(aAnyPtr.myTypeInfo),
	//	myPtr(aAnyPtr.myPtr)
	//{
	//}
	//inline AnyPointer::AnyPointer(AnyPointer&& aAnyPtr) noexcept :
	//	myTypeInfo(std::move(aAnyPtr.myTypeInfo)),
	//	myPtr(std::move(aAnyPtr.myPtr))
	//{
	//}
	template<class _Ty>
		requires(not std::is_same_v<_Ty, void>)
	inline AnyPointer::AnyPointer(_Ty* aPtr) noexcept :
		myTypeInfo(aPtr == nullptr ? nullptr : &typeid(_Ty)),
		myPtr(aPtr)
	{
	}

	template<class _Ty>
	inline _Ty* AnyPointer::TryGet() const noexcept
	{
		constexpr const std::type_info& typeInfo = typeid(_Ty);

		if (typeInfo == *myTypeInfo)
		{
			return reinterpret_cast<_Ty*>(myPtr);
		}
		return nullptr;
	}
	inline void* AnyPointer::Get() const noexcept
	{
		return myPtr;
	}
	inline const std::type_info* AnyPointer::GetTypeInfo() const noexcept
	{
		return myTypeInfo;
	}
	inline bool AnyPointer::IsNull() const noexcept
	{
		return myPtr == nullptr;
	}
	template<class _Ty>
	inline AnyPointer::operator _Ty* () const noexcept
	{
		constexpr const std::type_info& typeInfo = typeid(_Ty);

		if (typeInfo == *myTypeInfo)
		{
			return reinterpret_cast<_Ty*>(myPtr);
		}
		return nullptr;
	}
	inline AnyPointer::operator bool() const noexcept
	{
		return not IsNull();
	}
}