#pragma once
#include <utility>
#include <type_traits>
#include <new>

namespace CommonUtilities
{
	template<typename _Ty>
	class Nullable
	{
	public:
		Nullable();
		Nullable(Nullable<_Ty>&& aNullable);
		Nullable(const Nullable<_Ty>& aNullable);
		Nullable(_Ty&& aValue);
		Nullable(const _Ty& aValue);
		~Nullable();

		_Ty* Get();
		const _Ty* Get() const;

		bool IsNull() const;
		void SetNull();

		operator _Ty& ();
		operator const _Ty& () const;
		_Ty& operator*();
		const _Ty& operator*() const;
		_Ty* operator->();
		const _Ty* operator->() const;
		Nullable<_Ty>& operator=(_Ty&& aValue);
		Nullable<_Ty>& operator=(const _Ty& aValue);
		bool operator==(const _Ty& aValue) const;

	private:
		template<typename U = _Ty>
		typename std::enable_if<std::is_destructible<U>::value>::type _destroy();
		template<typename U = _Ty>
		typename std::enable_if<std::is_destructible<U>::value == false>::type _destroy();

		alignas(_Ty) unsigned char myData[sizeof(_Ty)];
		bool myIsNull;
	};
	template<typename _Ty>
	inline Nullable<_Ty>::Nullable() :
		myIsNull(true)
	{
	}
	template<typename _Ty>
	inline Nullable<_Ty>::Nullable(Nullable<_Ty>&& aNullable) : 
		myIsNull(std::move(aNullable.myIsNull))
	{
		::new(reinterpret_cast<void*>(myData)) _Ty(std::move(*reinterpret_cast<_Ty*>(aNullable.myData)));
	}
	template<typename _Ty>
	inline Nullable<_Ty>::Nullable(const Nullable<_Ty>& aNullable) :
		myIsNull(aNullable.myIsNull)
	{
		::new(reinterpret_cast<void*>(myData)) _Ty(*reinterpret_cast<const _Ty*>(aNullable.myData));
	}
	template<typename _Ty>
	inline Nullable<_Ty>::Nullable(_Ty&& aValue) : 
		myIsNull(false)
	{
		::new(reinterpret_cast<void*>(myData)) _Ty(aValue);
	}
	template<typename _Ty>
	inline Nullable<_Ty>::Nullable(const _Ty& aValue) : 
		myIsNull(false)
	{
		::new(reinterpret_cast<void*>(myData)) _Ty(aValue);
	}
	template<typename _Ty>
	inline Nullable<_Ty>::~Nullable()
	{
		if (IsNull() == false)
		{
			_destroy();
		}
	}
	template<typename _Ty>
	inline _Ty* Nullable<_Ty>::Get()
	{
		if (IsNull())
		{
			return nullptr;
		}
		else
		{
			return reinterpret_cast<_Ty*>(myData);
		}
	}
	template<typename _Ty>
	inline const _Ty* Nullable<_Ty>::Get() const
	{
		if (IsNull())
		{
			return nullptr;
		}
		else
		{
			return reinterpret_cast<const _Ty*>(myData);
		}
	}
	template<typename _Ty>
	inline bool Nullable<_Ty>::IsNull() const
	{
		return myIsNull;
	}
	template<typename _Ty>
	inline void Nullable<_Ty>::SetNull()
	{
		if (IsNull() == false)
		{
			_destroy();
		}
		myIsNull = true;
	}
	template<typename _Ty>
	inline Nullable<_Ty>::operator _Ty& ()
	{
		return *Get();
	}
	template<typename _Ty>
	inline Nullable<_Ty>::operator const _Ty& () const
	{
		return *Get();
	}
	template<typename _Ty>
	inline _Ty& Nullable<_Ty>::operator*()
	{
		return *Get();
	}
	template<typename _Ty>
	inline const _Ty& Nullable<_Ty>::operator*() const
	{
		return *Get();
	}
	template<typename _Ty>
	inline _Ty* Nullable<_Ty>::operator->()
	{
		return Get();
	}
	template<typename _Ty>
	inline const _Ty* Nullable<_Ty>::operator->() const
	{
		return Get();
	}
	template<typename _Ty>
	inline Nullable<_Ty>& Nullable<_Ty>::operator=(_Ty&& aValue)
	{
		if (IsNull())
		{
			::new(reinterpret_cast<void*>(myData)) _Ty(aValue);
		}
		else
		{
			*Get() = aValue;
		}
		return *this;
	}
	template<typename _Ty>
	inline Nullable<_Ty>& Nullable<_Ty>::operator=(const _Ty& aValue)
	{
		if (IsNull())
		{
			::new(reinterpret_cast<void*>(myData)) _Ty(aValue);
		}
		else
		{
			*Get() = aValue;
		}
		return *this;
	}
	template<typename _Ty>
	inline bool Nullable<_Ty>::operator==(const _Ty& aValue) const
	{
		return IsNull() == false && *Get() == aValue;
	}
	template<typename _Ty>
	template<typename U>
	inline typename std::enable_if<std::is_destructible<U>::value>::type Nullable<_Ty>::_destroy()
	{
		Get()->~_Ty();
	}
	template<typename _Ty>
	template<typename U>
	inline typename std::enable_if<std::is_destructible<U>::value == false>::type Nullable<_Ty>::_destroy()
	{
	}
}