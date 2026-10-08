#pragma once
#include "Defines.h"
#include "RefObject.hpp"

#include <type_traits>

namespace CommonUtilities
{
	template<class _Ty>
	class PImpl
	{
	public:
		using ValueType = _Ty;
		using Reference = ValueType&;
		using Pointer = ValueType*;

		PImpl() noexcept;
		PImpl(std::nullptr_t) noexcept;
		PImpl(Pointer aRefObject) noexcept;
		PImpl(const PImpl<_Ty>& aPImpl) noexcept;
		PImpl(PImpl<_Ty>&& aPImpl) noexcept;
		~PImpl();

		CU_NO_DISCARD _Ty* Get() const noexcept;

		template<class _OTy>
		CU_NO_DISCARD bool As(_OTy** outRef) const noexcept;

		CU_NO_DISCARD bool Expired() const noexcept;
		void Reset();

		CU_NO_DISCARD PImpl* GetAddressOf() noexcept;
		CU_NO_DISCARD const PImpl* GetAddressOf() const noexcept;

		CU_NO_DISCARD Pointer operator->() const noexcept;
		CU_NO_DISCARD Reference operator*() const;
		CU_NO_DISCARD const Pointer* operator&() const noexcept;
		CU_NO_DISCARD Pointer* operator&() noexcept;

		CU_NO_DISCARD operator bool() const noexcept;

		PImpl& operator=(std::nullptr_t) noexcept;
		PImpl& operator=(Pointer aRefObject) noexcept;
		PImpl& operator=(PImpl<_Ty> aPImpl) noexcept;
		PImpl& operator=(PImpl<_Ty>&& aPImpl) noexcept;

		template<class _OTy>
			requires(std::is_base_of_v<_Ty, _OTy>)
		PImpl& operator=(PImpl<_OTy> aPImpl) noexcept;
		template<class _OTy>
			requires(std::is_base_of_v<_Ty, _OTy>)
		PImpl& operator=(PImpl<_OTy>&& aPImpl) noexcept;

	private:
		Pointer myRefObject;
	};


	template<class _Ty>
	inline PImpl<_Ty>::PImpl() noexcept :
		myRefObject(nullptr)
	{
	}
	template<class _Ty>
	inline PImpl<_Ty>::PImpl(std::nullptr_t) noexcept :
		myRefObject(nullptr)
	{
	}
	template<class _Ty>
	inline PImpl<_Ty>::PImpl(Pointer aRefObject) noexcept :
		myRefObject(aRefObject)
	{
		if (myRefObject != nullptr)
		{
			myRefObject->Capture();
		}
	}
	template<class _Ty>
	inline PImpl<_Ty>::PImpl(const PImpl<_Ty>& aPImpl) noexcept :
		myRefObject(aPImpl.myRefObject)
	{
		if (myRefObject != nullptr)
		{
			myRefObject->Capture();
		}
	}
	template<class _Ty>
	inline PImpl<_Ty>::PImpl(PImpl<_Ty>&& aPImpl) noexcept :
		myRefObject(aPImpl.myRefObject)
	{
		aPImpl.myRefObject = nullptr;
	}
	template<class _Ty>
	inline PImpl<_Ty>::~PImpl()
	{
		Reset();
	}

	template<class _Ty>
	CU_NO_DISCARD inline _Ty* PImpl<_Ty>::Get() const noexcept
	{
		return myRefObject;
	}

	template<class _Ty>
	template<class _OTy>
	CU_NO_DISCARD inline bool PImpl<_Ty>::As(_OTy** outRef) const noexcept
	{
		if (*outRef != nullptr)
		{
			(**outRef).Release();
		}

		*outRef = static_cast<_OTy*>(myRefObject);
		(**outRef).Capture();
	}

	template<class _Ty>
	CU_NO_DISCARD inline bool PImpl<_Ty>::Expired() const noexcept
	{
		return myRefObject == nullptr;
	}
	template<class _Ty>
	inline void PImpl<_Ty>::Reset()
	{
		if (myRefObject != nullptr)
		{
			myRefObject->Release();
			myRefObject = nullptr;
		}
	}

	template<class _Ty>
	CU_NO_DISCARD inline PImpl<_Ty>* PImpl<_Ty>::GetAddressOf() noexcept
	{
		return this;
	}
	template<class _Ty>
	CU_NO_DISCARD inline const PImpl<_Ty>* PImpl<_Ty>::GetAddressOf() const noexcept
	{
		return this;
	}

	template<class _Ty>
	CU_NO_DISCARD inline PImpl<_Ty>::Pointer PImpl<_Ty>::operator->() const noexcept
	{
		return myRefObject;
	}
	template<class _Ty>
	CU_NO_DISCARD inline PImpl<_Ty>::Reference PImpl<_Ty>::operator*() const
	{
		return *myRefObject;
	}
	template<class _Ty>
	CU_NO_DISCARD inline const PImpl<_Ty>::Pointer* PImpl<_Ty>::operator&() const noexcept
	{
		return &myRefObject;
	}
	template<class _Ty>
	CU_NO_DISCARD inline PImpl<_Ty>::Pointer* PImpl<_Ty>::operator&() noexcept
	{
		return &myRefObject;
	}

	template<class _Ty>
	CU_NO_DISCARD inline PImpl<_Ty>::operator bool() const noexcept
	{
		return not Expired();
	}

	template<class _Ty>
	inline PImpl<_Ty>& PImpl<_Ty>::operator=(std::nullptr_t) noexcept
	{
		Reset();
		return *this;
	}
	template<class _Ty>
	inline PImpl<_Ty>& PImpl<_Ty>::operator=(Pointer aRefObject) noexcept
	{
		if (myRefObject != nullptr)
		{
			myRefObject->Release();
		}
		myRefObject = aRefObject;
		if (myRefObject != nullptr)
		{
			myRefObject->Capture();
		}
		return *this;
	}
	template<class _Ty>
	inline PImpl<_Ty>& PImpl<_Ty>::operator=(PImpl<_Ty> aPImpl) noexcept
	{
		if (myRefObject != nullptr)
		{
			myRefObject->Release();
		}
		myRefObject = aPImpl.myRefObject;
		if (myRefObject != nullptr)
		{
			myRefObject->Capture();
		}
		return *this;
	}
	template<class _Ty>
	inline PImpl<_Ty>& PImpl<_Ty>::operator=(PImpl<_Ty>&& aPImpl) noexcept
	{
		if (myRefObject != nullptr)
		{
			myRefObject->Release();
		}
		myRefObject = aPImpl.myRefObject;
		aPImpl.myRefObject = nullptr;
		return *this;
	}

	template<class _Ty>
	template<class _OTy>
		requires(std::is_base_of_v<_Ty, _OTy>)
	inline PImpl<_Ty>& PImpl<_Ty>::operator=(PImpl<_OTy> aPImpl) noexcept
	{
		if (myRefObject != nullptr)
		{
			myRefObject->Release();
		}
		myRefObject = static_cast<Pointer>(aPImpl.myRefObject);
		if (myRefObject != nullptr)
		{
			myRefObject->Capture();
		}
		return *this;
	}
	template<class _Ty>
	template<class _OTy>
		requires(std::is_base_of_v<_Ty, _OTy>)
	inline PImpl<_Ty>& PImpl<_Ty>::operator=(PImpl<_OTy>&& aPImpl) noexcept
	{
		if (myRefObject != nullptr)
		{
			myRefObject->Release();
		}
		myRefObject = static_cast<Pointer>(aPImpl.myRefObject);
		aPImpl.myRefObject = nullptr;
		return *this;
	}


	//template<typename _Ty>
	//class PImpl
	//{
	//	//static_assert(std::is_base_of_v<RefObject, _Ty> || std::is_same_v<RefObject, _Ty>, "_Ty must be derived from RefObject!");
	//public:
	//	PImpl() noexcept;
	//	PImpl(_Ty* aPtr);
	//	PImpl(nullptr_t) noexcept;
	//	PImpl(const PImpl<_Ty>& aPImpl) noexcept;
	//	PImpl(PImpl<_Ty>&& aPImpl) noexcept;

	//	~PImpl() noexcept;

	//	template<typename _CastTy>
	//	bool As(_CastTy** outPtr) const;

	//	_Ty* Get();
	//	const _Ty* Get() const;

	//	void Reset();

	//	template<typename _CastTy>
	//	explicit operator PImpl<_CastTy>();

	//	explicit operator _Ty* () noexcept;
	//	explicit operator const _Ty* () const noexcept;
	//	operator bool() const noexcept;

	//	bool operator==(_Ty* aLHS) const noexcept;
	//	bool operator==(PImpl<_Ty> aLHS) const noexcept;
	//	bool operator==(std::nullptr_t) const noexcept;
	//	bool operator!=(_Ty* aLHS) const noexcept;
	//	bool operator!=(PImpl<_Ty> aLHS) const noexcept;
	//	bool operator!=(std::nullptr_t) const noexcept;

	//	PImpl<_Ty>& operator=(_Ty* aLHS);
	//	PImpl<_Ty>& operator=(nullptr_t) noexcept;
	//	PImpl<_Ty>& operator=(PImpl<_Ty>&& aLHS);
	//	PImpl<_Ty>& operator=(const PImpl<_Ty>& aLHS);

	//	_Ty& operator*() const noexcept;
	//	_Ty* operator->() const noexcept;
	//	_Ty* const* operator&() const noexcept;
	//	_Ty** operator&() noexcept;

	//private:
	//	_Ty* myRefObject;
	//};
	//template<typename _Ty>
	//inline PImpl<_Ty>::PImpl() noexcept :
	//	myRefObject(nullptr)
	//{
	//}
	//template<typename _Ty>
	//inline PImpl<_Ty>::PImpl(_Ty* aPtr) :
	//	myRefObject(aPtr)
	//{
	//	if (myRefObject)
	//	{
	//		myRefObject->AddReference();
	//	}
	//}
	//template<typename _Ty>
	//inline PImpl<_Ty>::PImpl(nullptr_t) noexcept :
	//	myRefObject(nullptr)
	//{
	//}
	//template<typename _Ty>
	//inline PImpl<_Ty>::PImpl(const PImpl<_Ty>& aPImpl) noexcept :
	//	myRefObject(aPImpl.myRefObject)
	//{
	//	if (myRefObject)
	//	{
	//		myRefObject->AddReference();
	//	}
	//}
	//template<typename _Ty>
	//inline PImpl<_Ty>::PImpl(PImpl<_Ty>&& aPImpl) noexcept :
	//	myRefObject(aPImpl.myRefObject)
	//{
	//	aPImpl.myRefObject = nullptr;
	//}
	//template<typename _Ty>
	//inline PImpl<_Ty>::~PImpl() noexcept
	//{
	//	if (myRefObject)
	//	{
	//		myRefObject->RemoveReference();
	//	}
	//}
	//template<typename _Ty>
	//template<typename _CastTy>
	//inline bool PImpl<_Ty>::As(_CastTy** outPtr) const
	//{
	//	if (outPtr)
	//	{
	//		_CastTy* ptr = dynamic_cast<_CastTy*>(myRefObject);

	//		if (ptr)
	//		{
	//			*outPtr = ptr;
	//			myRefObject->AddReference();
	//			return true;
	//		}
	//	}
	//	return false;
	//}
	//template<typename _Ty>
	//inline _Ty* PImpl<_Ty>::Get()
	//{
	//	return myRefObject;
	//}
	//template<typename _Ty>
	//inline const _Ty* PImpl<_Ty>::Get() const
	//{
	//	return myRefObject;
	//}
	//template<typename _Ty>
	//inline void PImpl<_Ty>::Reset()
	//{
	//	if (myRefObject)
	//	{
	//		myRefObject->RemoveReference();
	//	}
	//	myRefObject = nullptr;
	//}
	//template<typename _Ty>
	//template<typename _CastTy>
	//inline PImpl<_Ty>::operator PImpl<_CastTy>()
	//{
	//	static_assert(std::is_base_of_v<_CastTy, _Ty>, "_Ty must be derived from _CastTy!");
	//	return PImpl<_CastTy>(myRefObject);
	//}
	//template<typename _Ty>
	//inline PImpl<_Ty>::operator _Ty* () noexcept
	//{
	//	return Get();
	//}
	//template<typename _Ty>
	//inline PImpl<_Ty>::operator const _Ty* () const noexcept
	//{
	//	return Get();
	//}
	//template<typename _Ty>
	//inline PImpl<_Ty>::operator bool() const noexcept
	//{
	//	return myRefObject != nullptr;
	//}
	//template<typename _Ty>
	//inline bool PImpl<_Ty>::operator==(_Ty* aLHS) const noexcept
	//{
	//	return myRefObject == aLHS;
	//}
	//template<typename _Ty>
	//inline bool PImpl<_Ty>::operator==(PImpl<_Ty> aLHS) const noexcept
	//{
	//	return myRefObject == aLHS.myRefObject;
	//}
	//template<typename _Ty>
	//inline bool PImpl<_Ty>::operator==(std::nullptr_t) const noexcept
	//{
	//	return myRefObject == nullptr;
	//}
	//template<typename _Ty>
	//inline bool PImpl<_Ty>::operator!=(_Ty* aLHS) const noexcept
	//{
	//	return myRefObject != aLHS;
	//}
	//template<typename _Ty>
	//inline bool PImpl<_Ty>::operator!=(PImpl<_Ty> aLHS) const noexcept
	//{
	//	return myRefObject != aLHS.myRefObject;
	//}
	//template<typename _Ty>
	//inline bool PImpl<_Ty>::operator!=(std::nullptr_t) const noexcept
	//{
	//	return myRefObject != nullptr;
	//}
	//template<typename _Ty>
	//inline PImpl<_Ty>& PImpl<_Ty>::operator=(_Ty* aLHS)
	//{
	//	if (aLHS)
	//	{
	//		aLHS->AddReference();
	//	}
	//	if (myRefObject)
	//	{
	//		myRefObject->RemoveReference();
	//	}
	//	myRefObject = aLHS;
	//	return *this;
	//}
	//template<typename _Ty>
	//inline PImpl<_Ty>& PImpl<_Ty>::operator=(nullptr_t) noexcept
	//{
	//	if (myRefObject)
	//	{
	//		myRefObject->RemoveReference();
	//	}
	//	myRefObject = nullptr;
	//	return *this;
	//}
	//template<typename _Ty>
	//inline PImpl<_Ty>& PImpl<_Ty>::operator=(PImpl<_Ty>&& aLHS)
	//{
	//	if (myRefObject)
	//	{
	//		myRefObject->RemoveReference();
	//	}
	//	myRefObject = aLHS.myRefObject;
	//	aLHS.myRefObject = nullptr;
	//	return *this;
	//}
	//template<typename _Ty>
	//inline PImpl<_Ty>& PImpl<_Ty>::operator=(const PImpl<_Ty>& aLHS)
	//{
	//	if (aLHS.myRefObject)
	//	{
	//		aLHS.myRefObject->AddReference();
	//	}
	//	if (myRefObject)
	//	{
	//		myRefObject->RemoveReference();
	//	}
	//	myRefObject = aLHS.myRefObject;
	//	return *this;
	//}
	//template<typename _Ty>
	//inline _Ty& PImpl<_Ty>::operator*() const noexcept
	//{
	//	return *myRefObject;
	//}
	//template<typename _Ty>
	//inline _Ty* PImpl<_Ty>::operator->() const noexcept
	//{
	//	return myRefObject;
	//}
	//template<typename _Ty>
	//inline _Ty* const* PImpl<_Ty>::operator&() const noexcept
	//{
	//	return &myRefObject;
	//}
	//template<typename _Ty>
	//inline _Ty** PImpl<_Ty>::operator&() noexcept
	//{
	//	return &myRefObject;
	//}
}