#pragma once
#include "Registry.hpp"
#include <utility>

namespace CommonUtilities
{
	template<typename _IDTy>
	class IDObject
	{
	public:
		typedef _IDTy ID;

		IDObject();
		virtual ~IDObject() = default;

		template<typename _IDObjectTy, typename ..._Args>
		static _IDObjectTy* Create(_Args&&... aArgs);

		static IDObject* Find(ID aID);
		static bool Destroy(ID aID);

		void AddRef();
		void RemoveRef();
		unsigned int GetRefCount() const;

		ID GetID() const;

	private:
		static Registry<ID, IDObject*> ourIDObjects;
		static ID ourNextID;

		ID myID;
		unsigned int myRefCount;
	};

	template<typename _IDTy, bool _AddRef = true>
	class IDObjectRef
	{
	public:
		typedef _IDTy ID;

		IDObjectRef() noexcept;
		IDObjectRef(ID aID);
		IDObjectRef(IDObject<ID>* aObject);
		IDObjectRef(nullptr_t aNullptr) noexcept;
		IDObjectRef(const IDObjectRef& aObjectRef) noexcept;
		IDObjectRef(IDObjectRef&& aObjectRef) noexcept;
		~IDObjectRef() noexcept;

		template<typename _CastTy>
		bool As(_CastTy** outPtr) const;

		IDObject<ID>* Get();
		const IDObject<ID>* Get() const;

		void Reset();

		template<bool _OtherAddRef>
		explicit operator IDObjectRef<_IDTy, _OtherAddRef>();

		explicit operator _Ty* () noexcept;
		explicit operator const _Ty* () const noexcept;
		operator bool() const noexcept;

		bool operator==(IDObject<ID>* aLHS) const noexcept;
		bool operator==(IDObjectRef aLHS) const noexcept;
		bool operator==(std::nullptr_t) const noexcept;
		bool operator!=(IDObject<ID>* aLHS) const noexcept;
		bool operator!=(IDObjectRef aLHS) const noexcept;
		bool operator!=(std::nullptr_t) const noexcept;

		IDObjectRef& operator=(IDObject<ID>* aLHS);
		IDObjectRef& operator=(nullptr_t) noexcept;
		IDObjectRef& operator=(IDObjectRef&& aObjectRef);
		IDObjectRef& operator=(const IDObjectRef& aObjectRef);

		IDObject<ID>* operator->() const noexcept;
		IDObject<ID>* const* operator&() const noexcept;
		IDObject<ID>** operator&() noexcept;

	private:
	};

	template<typename _IDTy>
	Registry<typename IDObject<_IDTy>::ID, IDObject<_IDTy>*> IDObject<_IDTy>::ourIDObjects = {};

	template<typename _IDTy>
	typename IDObject<_IDTy>::ID IDObject<_IDTy>::ourNextID = static_cast<typename IDObject<_IDTy>::ID>(1);


	template<typename _IDTy>
	inline IDObject<_IDTy>::IDObject() : 
		myID(ourNextID++),
		myRefCount(0u)
	{
	}

	template<typename _IDTy>
	inline IDObject<_IDTy>* IDObject<_IDTy>::Find(ID aID)
	{
		return ourIDObjects.find(aID);
	}

	template<typename _IDTy>
	inline bool IDObject<_IDTy>::Destroy(ID aID)
	{
		IDObject* object = ourIDObjects.find(aID);
		if (object != nullptr)
		{
			ourIDObjects.erase(aID)
				delete object;
			return true;
		}
		return false;
	}

	template<typename _IDTy>
	inline void CommonUtilities::IDObject<_IDTy>::AddRef()
	{
		++myRefCount;
	}

	template<typename _IDTy>
	inline void CommonUtilities::IDObject<_IDTy>::RemoveRef()
	{
		--myRefCount;
		if (myRefCount <= 0)
		{
			IDObject<_IDTy>::Destroy(GetID());
		}
	}

	template<typename _IDTy>
	inline unsigned int CommonUtilities::IDObject<_IDTy>::GetRefCount() const
	{
		return myRefCount;
	}

	template<typename _IDTy>
	inline typename IDObject<_IDTy>::ID IDObject<_IDTy>::GetID() const
	{
		return myID;
	}

	template<typename _IDTy>
	template<typename _IDObjectTy, typename ..._Args>
	inline _IDObjectTy* IDObject<_IDTy>::Create(_Args&&... aArgs)
	{
		_IDObjectTy* object = new _IDObjectTy(std::forward(aArgs)...);
		ourIDObjects->insert(object->GetID(), object);
		return object;
	}
}