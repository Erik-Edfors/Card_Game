#pragma once

namespace CommonUtilities
{
	class RefObject
	{
	public:
		virtual void Capture() = 0;
		virtual void Release() = 0;
		virtual size_t GetRefCount() const = 0;
	};

	template<class _Ty>
	class RefWrapper;

	template<class _Ty>
	bool CreateRefWrapper(RefWrapper<_Ty>** outWrapper);
	template<class _Ty>
	bool CreateRefWrapper(RefWrapper<_Ty>** outWrapper, const _Ty& aObject);
	template<class _Ty>
	bool CreateRefWrapper(RefWrapper<_Ty>** outWrapper, _Ty&& aObject);

	template<class _Ty>
	class RefWrapper : public RefObject
	{
		friend bool CreateRefWrapper<_Ty>(RefWrapper<_Ty>** outWrapper);
		friend bool CreateRefWrapper<_Ty>(RefWrapper<_Ty>** outWrapper, const _Ty& aObject);
		friend bool CreateRefWrapper<_Ty>(RefWrapper<_Ty>** outWrapper, _Ty&& aObject);
	public:
		~RefWrapper();

		RefWrapper(const RefWrapper&) = delete;
		RefWrapper(RefWrapper&&) = delete;

		virtual void Capture() override;
		virtual void Release() override;
		virtual size_t GetRefCount() const override;

		const _Ty& Get() const noexcept;
		_Ty& Get() noexcept;

	private:
		RefWrapper();
		RefWrapper(const _Ty& aObject);
		RefWrapper(_Ty&& aObject);

		_Ty myObject;
		size_t myRefCount;
	};

	template<class _Ty>
	bool CreateRefWrapper(RefWrapper<_Ty>** outWrapper)
	{
		if (*outWrapper != nullptr)
		{
			(**outWrapper).Release();
		}
		*outWrapper = new RefWrapper<_Ty>();
		(**outWrapper).Capture();
		return true;
	}
	template<class _Ty>
	bool CreateRefWrapper(RefWrapper<_Ty>** outWrapper, const _Ty& aObject)
	{
		if (*outWrapper != nullptr)
		{
			(**outWrapper).Release();
		}
		*outWrapper = new RefWrapper<_Ty>(aObject);
		(**outWrapper).Capture();
		return true;
	}
	template<class _Ty>
	bool CreateRefWrapper(RefWrapper<_Ty>** outWrapper, _Ty&& aObject)
	{
		if (*outWrapper != nullptr)
		{
			(**outWrapper).Release();
		}
		*outWrapper = new RefWrapper<_Ty>(std::move(aObject));
		(**outWrapper).Capture();
		return true;
	}

	template<class _Ty>
	inline RefWrapper<_Ty>::RefWrapper() : 
		myRefCount(0)
	{
	}
	template<class _Ty>
	inline RefWrapper<_Ty>::RefWrapper(const _Ty& aObject) :
		myObject(aObject),
		myRefCount(0)
	{
	}
	template<class _Ty>
	inline RefWrapper<_Ty>::RefWrapper(_Ty&& aObject) :
		myObject(std::move(aObject)),
		myRefCount(0)
	{
	}
	template<class _Ty>
	inline RefWrapper<_Ty>::~RefWrapper()
	{
	}

	template<class _Ty>
	inline void RefWrapper<_Ty>::Capture()
	{
		++myRefCount;
	}
	template<class _Ty>
	inline void RefWrapper<_Ty>::Release()
	{
		--myRefCount;
		if (myRefCount == 0)
		{
			delete this;
		}
	}
	template<class _Ty>
	inline size_t RefWrapper<_Ty>::GetRefCount() const
	{
		return myRefCount;
	}

	template<class _Ty>
	inline const _Ty& RefWrapper<_Ty>::Get() const noexcept
	{
		return myObject;
	}
	template<class _Ty>
	inline _Ty& RefWrapper<_Ty>::Get() noexcept
	{
		return myObject;
	}

	//class RefObject
	//{
	//public:
	//	RefObject();
	//	virtual ~RefObject() = default;

	//	void AddReference() noexcept;
	//	void RemoveReference() noexcept;

	//	int ReferenceCount() const noexcept;

	//	template<typename _Ty>
	//	bool As(_Ty** outPtr) noexcept;
	//private:
	//	int myReferenceCount;
	//};
	//inline RefObject::RefObject() : 
	//	myReferenceCount(0)
	//{
	//}
	//inline void RefObject::AddReference() noexcept
	//{
	//	++myReferenceCount;
	//}
	//inline void RefObject::RemoveReference() noexcept
	//{
	//	--myReferenceCount;
	//	if (myReferenceCount <= 0)
	//	{
	//		delete this;
	//	}
	//}
	//inline int RefObject::ReferenceCount() const noexcept
	//{
	//	return myReferenceCount;
	//}
	//template<typename _Ty>
	//inline bool RefObject::As(_Ty** outPtr) noexcept
	//{
	//	if (outPtr == nullptr)
	//	{
	//		return false;
	//	}
	//	_Ty* ptr = dynamic_cast<_Ty*>(this);
	//	if (ptr != nullptr)
	//	{
	//		*outPtr = ptr;
	//		this->AddReference();
	//		return true;
	//	}
	//	else
	//	{
	//		return false;
	//	}
	//}
}