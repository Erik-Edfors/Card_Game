#include "ComponentSystem.pch.h"
#include "Component.h"
#include "ComponentRef.hpp"

namespace cs
{
	Component::Component() :
		mySystem(nullptr),
		myHandle({}),
		myEntityHandle({}),
		myIsDestroyed(false),
		myIsStarted(false)
	{
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
	ComponentSystem& Component::GetSystem() const noexcept
	{
		return *mySystem;
	}
	ComponentHandle Component::GetHandle() const noexcept
	{
		return myHandle;
	}
	EntityHandle Component::GetEntityHandle() const noexcept
	{
		return myEntityHandle;
	}
	void Component::Initialize(ComponentSystem* aSystem, ComponentHandle aHandle, EntityHandle aEntityHandle)
	{
		mySystem = aSystem;
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
		OnUpdate(aDeltaTime);
	}
	void Component::LateUpdate(float aDeltaTime)
	{
		OnLateUpdate(aDeltaTime);
	}
}
