#include "ComponentSystem.pch.h"
#include "Component.h"
#include "ComponentRef.hpp"

namespace cs
{
	Component::Component() :
		myRegistry(nullptr),
		myHandle(INVALID_COMPONENT_HANDLE),
		myEntityHandle(INVALID_ENTITY_HANDLE),
		myEnabled(true),
		myIsDestroyed(false),
		myIsStarted(false)
	{
	}
	void Component::SetEnabled(bool aEnabled) noexcept
	{
		if (myEnabled != aEnabled)
		{
			myEnabled = aEnabled;
			if (aEnabled)
			{
				OnEnable();
			}
			else
			{
				OnDisable();
			}
		}
	}
	bool Component::IsEnabled() const noexcept
	{
		return myEnabled;
	}
	void Component::Destroy()
	{
		if (not myIsDestroyed)
		{
			myIsDestroyed = true;
			OnDestroy();
		}
	}
	bool Component::IsDestroyed() const noexcept
	{
		return myIsDestroyed;
	}
	bool Component::IsStarted() const noexcept
	{
		return myIsStarted;
	}
	ComponentRegistryBase& Component::GetRegistry() const noexcept
	{
		return *myRegistry;
	}
	ComponentSystem& Component::GetSystem() const noexcept
	{
		return GetRegistry().GetSystem();
	}
	ComponentHandle Component::GetHandle() const noexcept
	{
		return myHandle;
	}
	EntityHandle Component::GetEntityHandle() const noexcept
	{
		return myEntityHandle;
	}
	void Component::Initialize(ComponentRegistryBase* aRegistry, ComponentHandle aHandle, EntityHandle aEntityHandle)
	{
		myRegistry = aRegistry;
		myHandle = aHandle;
		myEntityHandle = aEntityHandle;
		OnInit();
	}
	void Component::Start()
	{
		if (not IsStarted())
		{
			myIsStarted = true;
			OnStart();
		}
	}
	void Component::Update(float aDeltaTime)
	{
		if (IsEnabled())
		{
			OnUpdate(aDeltaTime);
		}
	}
	void Component::LateUpdate(float aDeltaTime)
	{
		if (IsEnabled())
		{
			OnLateUpdate(aDeltaTime);
		}
	}
}
