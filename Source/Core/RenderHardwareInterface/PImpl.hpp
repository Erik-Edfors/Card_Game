#pragma once
#include "RefObject.hpp"

namespace graphics
{
	template<typename _Ty>
	class PImpl
	{
		//static_assert(std::is_base_of_v<RefObject, _Ty> || std::is_same_v<RefObject, _Ty>, "_Ty must be derived from RefObject!");
	public:
		PImpl() noexcept;
		PImpl(_Ty* aPtr);
		PImpl(nullptr_t) noexcept;
		PImpl(const PImpl<_Ty>& aPImpl) noexcept;
		PImpl(PImpl<_Ty>&& aPImpl) noexcept;

		~PImpl() noexcept;

		template<typename _CastTy>
		bool As(_CastTy** outPtr) const;

		_Ty* Get() const noexcept;
		//const _Ty* Get() const;

		PImpl<_Ty>* GetAddressOf() noexcept;
		const PImpl<_Ty>* GetAddressOf() const noexcept;

		void Reset();

		template<typename _CastTy>
		explicit operator PImpl<_CastTy>();

		explicit operator _Ty* () noexcept;
		explicit operator const _Ty* () const noexcept;
		operator bool() const noexcept;

		bool operator==(_Ty* aLHS) const noexcept;
		bool operator==(PImpl<_Ty> aLHS) const noexcept;
		bool operator==(std::nullptr_t) const noexcept;
		bool operator!=(_Ty* aLHS) const noexcept;
		bool operator!=(PImpl<_Ty> aLHS) const noexcept;
		bool operator!=(std::nullptr_t) const noexcept;

		PImpl<_Ty>& operator=(_Ty* aLHS);
		PImpl<_Ty>& operator=(nullptr_t) noexcept;
		PImpl<_Ty>& operator=(PImpl<_Ty>&& aLHS) noexcept;
		PImpl<_Ty>& operator=(const PImpl<_Ty>& aLHS) noexcept;

		_Ty* operator->() const noexcept;
		_Ty* const* operator&() const noexcept;
		_Ty** operator&() noexcept;

	private:
		_Ty* myRefObject;
	};
	template<typename _Ty>
	inline PImpl<_Ty>::PImpl() noexcept :
		myRefObject(nullptr)
	{
	}
	template<typename _Ty>
	inline PImpl<_Ty>::PImpl(_Ty* aPtr) :
		myRefObject(aPtr)
	{
		if (myRefObject)
		{
			myRefObject->AddReference();
		}
	}
	template<typename _Ty>
	inline PImpl<_Ty>::PImpl(nullptr_t) noexcept :
		myRefObject(nullptr)
	{
	}
	template<typename _Ty>
	inline PImpl<_Ty>::PImpl(const PImpl<_Ty>& aPImpl) noexcept :
		myRefObject(aPImpl.myRefObject)
	{
		if (myRefObject)
		{
			myRefObject->AddReference();
		}
	}
	template<typename _Ty>
	inline PImpl<_Ty>::PImpl(PImpl<_Ty>&& aPImpl) noexcept :
		myRefObject(aPImpl.myRefObject)
	{
		aPImpl.myRefObject = nullptr;
	}
	template<typename _Ty>
	inline PImpl<_Ty>::~PImpl() noexcept
	{
		if (myRefObject)
		{
			myRefObject->RemoveReference();
		}
	}
	template<typename _Ty>
	template<typename _CastTy>
	inline bool PImpl<_Ty>::As(_CastTy** outPtr) const
	{
		if (outPtr)
		{
			if (*outPtr != nullptr)
			{
				(**outPtr).RemoveReference();
				*outPtr = nullptr;
			}

			_CastTy* ptr = dynamic_cast<_CastTy*>(myRefObject);

			if (ptr)
			{
				*outPtr = ptr;
				myRefObject->AddReference();
				return true;
			}
		}
		return false;
	}
	template<typename _Ty>
	inline _Ty* PImpl<_Ty>::Get() const noexcept
	{
		return myRefObject;
	}
	//template<typename _Ty>
	//inline const _Ty* PImpl<_Ty>::Get() const
	//{
	//	return myRefObject;
	//}
	template<typename _Ty>
	inline PImpl<_Ty>* PImpl<_Ty>::GetAddressOf() noexcept
	{
		return this;
	}
	template<typename _Ty>
	inline const PImpl<_Ty>* PImpl<_Ty>::GetAddressOf() const noexcept
	{
		return this;
	}
	template<typename _Ty>
	inline void PImpl<_Ty>::Reset()
	{
		if (myRefObject)
		{
			myRefObject->RemoveReference();
		}
		myRefObject = nullptr;
	}
	template<typename _Ty>
	template<typename _CastTy>
	inline PImpl<_Ty>::operator PImpl<_CastTy>()
	{
		static_assert(std::is_base_of_v<_CastTy, _Ty>, "_Ty must be derived from _CastTy!");
		return PImpl<_CastTy>(myRefObject);
	}
	template<typename _Ty>
	inline PImpl<_Ty>::operator _Ty* () noexcept
	{
		return Get();
	}
	template<typename _Ty>
	inline PImpl<_Ty>::operator const _Ty* () const noexcept
	{
		return Get();
	}
	template<typename _Ty>
	inline PImpl<_Ty>::operator bool() const noexcept
	{
		return myRefObject != nullptr;
	}
	template<typename _Ty>
	inline bool PImpl<_Ty>::operator==(_Ty* aLHS) const noexcept
	{
		return myRefObject == aLHS;
	}
	template<typename _Ty>
	inline bool PImpl<_Ty>::operator==(PImpl<_Ty> aLHS) const noexcept
	{
		return myRefObject == aLHS.myRefObject;
	}
	template<typename _Ty>
	inline bool PImpl<_Ty>::operator==(std::nullptr_t) const noexcept
	{
		return myRefObject == nullptr;
	}
	template<typename _Ty>
	inline bool PImpl<_Ty>::operator!=(_Ty* aLHS) const noexcept
	{
		return myRefObject != aLHS;
	}
	template<typename _Ty>
	inline bool PImpl<_Ty>::operator!=(PImpl<_Ty> aLHS) const noexcept
	{
		return myRefObject != aLHS.myRefObject;
	}
	template<typename _Ty>
	inline bool PImpl<_Ty>::operator!=(std::nullptr_t) const noexcept
	{
		return myRefObject != nullptr;
	}
	template<typename _Ty>
	inline PImpl<_Ty>& PImpl<_Ty>::operator=(_Ty* aLHS)
	{
		if (aLHS)
		{
			aLHS->AddReference();
		}
		if (myRefObject)
		{
			myRefObject->RemoveReference();
		}
		myRefObject = aLHS;
		return *this;
	}
	template<typename _Ty>
	inline PImpl<_Ty>& PImpl<_Ty>::operator=(nullptr_t) noexcept
	{
		if (myRefObject)
		{
			myRefObject->RemoveReference();
		}
		myRefObject = nullptr;
		return *this;
	}
	template<typename _Ty>
	inline PImpl<_Ty>& PImpl<_Ty>::operator=(PImpl<_Ty>&& aLHS) noexcept
	{
		if (myRefObject)
		{
			myRefObject->RemoveReference();
		}
		myRefObject = aLHS.myRefObject;
		aLHS.myRefObject = nullptr;
		return *this;
	}
	template<typename _Ty>
	inline PImpl<_Ty>& PImpl<_Ty>::operator=(const PImpl<_Ty>& aLHS) noexcept
	{
		if (aLHS.myRefObject)
		{
			aLHS.myRefObject->AddReference();
		}
		if (myRefObject)
		{
			myRefObject->RemoveReference();
		}
		myRefObject = aLHS.myRefObject;
		return *this;
	}
	template<typename _Ty>
	inline _Ty* PImpl<_Ty>::operator->() const noexcept
	{
		return myRefObject;
	}
	template<typename _Ty>
	inline _Ty* const* PImpl<_Ty>::operator&() const noexcept
	{
		return &myRefObject;
	}
	template<typename _Ty>
	inline _Ty** PImpl<_Ty>::operator&() noexcept
	{
		return &myRefObject;
	}
}