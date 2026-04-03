#pragma once
#include <type_traits>

namespace CommonUtilities
{
	class RefObject
	{
	public:
		RefObject();
		virtual ~RefObject() = default;

		void AddReference() noexcept;
		void RemoveReference() noexcept;

		int ReferenceCount() const noexcept;

		template<typename _Ty>
		bool As(_Ty** outPtr) noexcept;
	private:
		int myReferenceCount;
	};
	inline RefObject::RefObject() : 
		myReferenceCount(0)
	{
	}
	inline void RefObject::AddReference() noexcept
	{
		++myReferenceCount;
	}
	inline void RefObject::RemoveReference() noexcept
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