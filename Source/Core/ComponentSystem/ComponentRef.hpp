#pragma once
#include "Defines.h"
#include "ComponentSystem.h"
#include "ComponentRegistry.h"
#include <type_traits>

namespace cs
{
	template<class _ComponentTy>
	class ComponentRegistry;

	template<class _ComponentTy>
	class ComponentRef
	{
	public:
		using Value = _ComponentTy;
		using PureValue = std::remove_const_t<Value>;
		using Pointer = Value*;
		using Reference = Value&;
		using Registry = ComponentRegistry<PureValue>;

		ComponentRef() noexcept;
		ComponentRef(const ComponentRef& aRef) noexcept;
		ComponentRef(ComponentRef&& aRef) noexcept;
		ComponentRef(std::nullptr_t) noexcept;
		ComponentRef(Pointer aComponent) noexcept;
		ComponentRef(ComponentSystem* aSystem, ComponentHandle aHandle) noexcept;
		ComponentRef(Registry* aRegistry, ComponentHandle aHandle) noexcept;
		~ComponentRef() noexcept;

		Pointer Get() const noexcept;
		Registry* GetRegistry() const noexcept;
		ComponentSystem* GetSystem() const noexcept;

		void Reset() noexcept;

		bool Expired() const noexcept;

		Reference operator*() const noexcept;
		Pointer operator->() const noexcept;

		operator bool() const noexcept;
		operator Pointer() const noexcept;

		bool operator==(const ComponentRef& aRef) const noexcept;
		bool operator==(const std::remove_const_t<Pointer> aComponent) const noexcept;
		bool operator==(const std::nullptr_t) const noexcept;
		bool operator!=(const ComponentRef& aRef) const noexcept;
		bool operator!=(const std::remove_const_t<Pointer> aComponent) const noexcept;
		bool operator!=(const std::nullptr_t) const noexcept;

		ComponentRef& operator=(const ComponentRef& aRef) noexcept;
		ComponentRef& operator=(ComponentRef&& aRef) noexcept;
		ComponentRef& operator=(Pointer aComponent) noexcept;

	private:
		Registry* myRegistry;
		ComponentHandle myHandle;
	};



	template<class _ComponentTy>
	inline ComponentRef<_ComponentTy>::ComponentRef() noexcept :
		myRegistry(nullptr),
		myHandle(CS_INVALID_COMPONENT_HANDLE)
	{
	}
	template<class _ComponentTy>
	inline ComponentRef<_ComponentTy>::ComponentRef(const ComponentRef& aRef) noexcept :
		myRegistry(aRef.myRegistry),
		myHandle(aRef.myHandle)
	{
	}
	template<class _ComponentTy>
	inline ComponentRef<_ComponentTy>::ComponentRef(ComponentRef&& aRef) noexcept :
		myRegistry(std::move(aRef.myRegistry)),
		myHandle(std::move(aRef.myHandle))
	{
		aRef.myRegistry = nullptr;
		aRef.myHandle = CS_INVALID_COMPONENT_HANDLE;
	}
	template<class _ComponentTy>
	inline ComponentRef<_ComponentTy>::ComponentRef(std::nullptr_t) noexcept :
		myRegistry(nullptr),
		myHandle(CS_INVALID_COMPONENT_HANDLE)
	{
	}
	template<class _ComponentTy>
	inline ComponentRef<_ComponentTy>::ComponentRef(Pointer aComponent) noexcept :
		myRegistry(nullptr),
		myHandle(CS_INVALID_COMPONENT_HANDLE)
	{
		if (aComponent != nullptr)
		{
			myRegistry = aComponent->GetSystem().GetRegistry<PureValue>();
			if (myRegistry != nullptr)
			{
				myHandle = aComponent->GetHandle();
			}
		}
	}
	template<class _ComponentTy>
	inline ComponentRef<_ComponentTy>::ComponentRef(ComponentSystem* aSystem, ComponentHandle aHandle) noexcept :
		myRegistry(nullptr),
		myHandle(CS_INVALID_COMPONENT_HANDLE)
	{
		if (aSystem != nullptr)
		{
			myRegistry = aSystem->GetRegistry<PureValue>();
			if (myRegistry != nullptr)
			{
				myHandle = aHandle;
			}
		}
	}
	template<class _ComponentTy>
	inline ComponentRef<_ComponentTy>::ComponentRef(Registry* aRegistry, ComponentHandle aHandle) noexcept :
		myRegistry(nullptr),
		myHandle(CS_INVALID_COMPONENT_HANDLE)
	{
	}
	template<class _ComponentTy>
	inline ComponentRef<_ComponentTy>::~ComponentRef() noexcept
	{
	}

	template<class _ComponentTy>
	inline typename ComponentRef<_ComponentTy>::Pointer ComponentRef<_ComponentTy>::Get() const noexcept
	{
		if (myRegistry != nullptr)
		{
			return myRegistry->GetComponent(myHandle);
		}
		return nullptr;
	}
	template<class _ComponentTy>
	inline typename ComponentRef<_ComponentTy>::Registry* ComponentRef<_ComponentTy>::GetRegistry() const noexcept
	{
		return myRegistry;
	}
	template<class _ComponentTy>
	inline ComponentSystem* ComponentRef<_ComponentTy>::GetSystem() const noexcept
	{
		if (myRegistry != nullptr)
		{
			return &myRegistry->GetSystem();
		}
		return nullptr;
	}

	template<class _ComponentTy>
	inline void ComponentRef<_ComponentTy>::Reset() noexcept
	{
		myRegistry = nullptr;
		myHandle = CS_INVALID_COMPONENT_HANDLE;
	}

	template<class _ComponentTy>
	inline bool ComponentRef<_ComponentTy>::Expired() const noexcept
	{
		return Get() == nullptr;
	}


	template<class _ComponentTy>
	inline typename ComponentRef<_ComponentTy>::Reference ComponentRef<_ComponentTy>::operator*() const noexcept
	{
		return *Get();
	}
	template<class _ComponentTy>
	inline typename ComponentRef<_ComponentTy>::Pointer ComponentRef<_ComponentTy>::operator->() const noexcept
	{
		return Get();
	}

	template<class _ComponentTy>
	inline ComponentRef<_ComponentTy>::operator bool() const noexcept
	{
		return Get() != nullptr;
	}
	template<class _ComponentTy>
	inline ComponentRef<_ComponentTy>::operator Pointer() const noexcept
	{
		if (myRegistry != nullptr && myHandle != CS_INVALID_COMPONENT_HANDLE)
		{
			return myRegistry->GetComponent(myHandle);
		}
		else return nullptr;
	}

	template<class _ComponentTy>
	inline bool ComponentRef<_ComponentTy>::operator==(const ComponentRef& aRef) const noexcept
	{
		return myRegistry == aRef.myRegistry && myHandle == aRef.myHandle;
	}
	template<class _ComponentTy>
	inline bool ComponentRef<_ComponentTy>::operator==(const std::remove_const_t<Pointer> aComponent) const noexcept
	{
		return Get() == aComponent;
	}
	template<class _ComponentTy>
	inline bool ComponentRef<_ComponentTy>::operator==(std::nullptr_t) const noexcept
	{
		return Get() == nullptr;
	}
	template<class _ComponentTy>
	inline bool ComponentRef<_ComponentTy>::operator!=(const ComponentRef& aRef) const noexcept
	{
		return myRegistry != aRef.myRegistry && myHandle != aRef.myHandle;
	}
	template<class _ComponentTy>
	inline bool ComponentRef<_ComponentTy>::operator!=(const std::remove_const_t<Pointer> aComponent) const noexcept
	{
		return Get() != aComponent;
	}
	template<class _ComponentTy>
	inline bool ComponentRef<_ComponentTy>::operator!=(std::nullptr_t) const noexcept
	{
		return Get() != nullptr;
	}

	template<class _ComponentTy>
	inline ComponentRef<_ComponentTy>& ComponentRef<_ComponentTy>::operator=(const ComponentRef& aRef) noexcept
	{
		myRegistry = aRef.myRegistry;
		myHandle = aRef.myHandle;
		return *this;
	}
	template<class _ComponentTy>
	inline ComponentRef<_ComponentTy>& ComponentRef<_ComponentTy>::operator=(ComponentRef&& aRef) noexcept
	{
		myRegistry = std::move(aRef.myRegistry);
		myHandle = std::move(aRef.myHandle);

		aRef.myRegistry = nullptr;
		aRef.myHandle = CS_INVALID_COMPONENT_HANDLE;
		return *this;
	}
	template<class _ComponentTy>
	inline ComponentRef<_ComponentTy>& ComponentRef<_ComponentTy>::operator=(Pointer aComponent) noexcept
	{
		if (aComponent != nullptr)
		{
			myRegistry = aComponent->GetSystem().GetRegistry<PureValue>();
			if (myRegistry != nullptr)
			{
				myHandle = aComponent->GetHandle();
			}
		}
		else
		{
			myRegistry = nullptr;
			myHandle = CS_INVALID_COMPONENT_HANDLE;
		}
		return *this;
	}
}