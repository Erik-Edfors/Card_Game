#include "Shared.pch.h"
#include "Actor.h"

namespace shared
{
	Actor::Actor() :
		mySystem(nullptr),
		myID(INVALID_ACTOR),
		myIsDestroyed(false)
	{
	}
	Actor::~Actor()
	{
	}
	void Actor::Destroy()
	{
		if (not IsDestroyed())
		{
			myIsDestroyed = true;

			for (const ComponentRef<Component>& component : GetComponents())
			{
				component->Destroy();
			}
		}
	}
	bool Actor::IsDestroyed() const noexcept
	{
		return myIsDestroyed;
	}
	ActorID Actor::GetID() const noexcept
	{
		return myID;
	}
	ComponentSystem* Actor::GetSystem() const noexcept
	{
		return mySystem;
	}
	const std::vector<ComponentRef<Component>>& Actor::GetComponents() noexcept
	{
		std::erase_if(
			myComponents,
			[](const ComponentRef<Component>& aComponents) {
				return aComponents.Expired();
			}
		);

		return myComponents;
	}
	void Actor::Initialize(ComponentSystem* aSystem, ActorID aID)
	{
		myID = aID;
		mySystem = aSystem;
	}
	void Actor::InsertComponent(ComponentID aComponent)
	{
		myComponents.push_back(ComponentRef<Component>(GetSystem(), aComponent));
	}
}
