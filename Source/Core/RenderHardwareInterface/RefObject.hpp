#pragma once
#include <type_traits>

namespace graphics
{
	class RefObject
	{
	public:
		RefObject();
		RefObject(const RefObject& aRefObject);
		virtual ~RefObject() = default;

		void AddReference() const noexcept;
		void RemoveReference() const noexcept;

		int ReferenceCount() const noexcept;

		template<typename _Ty>
		bool As(_Ty** outPtr) noexcept;
	private:
		mutable int myReferenceCount;
	};
	inline RefObject::RefObject() :
		myReferenceCount(0)
	{
	}
	inline RefObject::RefObject(const RefObject&) : 
		myReferenceCount(0)
	{
	}
	inline void RefObject::AddReference() const noexcept
	{
		++myReferenceCount;
	}
	inline void RefObject::RemoveReference() const noexcept
	{
		--myReferenceCount;
		if (myReferenceCount <= 0)
		{
			delete this;
		}
	}
	inline int RefObject::ReferenceCount() const noexcept
	{
		return myReferenceCount;
	}
	template<typename _Ty>
	inline bool RefObject::As(_Ty** outPtr) noexcept
	{
		if (outPtr == nullptr)
		{
			return false;
		}

		_Ty* ptr = dynamic_cast<_Ty*>(this);
		if (ptr != nullptr)
		{
			*outPtr = ptr;
			this->AddReference();
			return true;
		}
		else
		{
			return false;
		}
	}
}