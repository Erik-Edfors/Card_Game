#include "Shared.pch.h"
#include "Component.h"

namespace shared
{
	Component::Component() :
		myID(INVALID_COMPONENT),
		myActor(),
		myIsDestroyed(false)
	{
	}
	Component::~Component()
	{
	}
	void Component::Destroy()
	{
		if (not IsDestroyed())
		{
			myIsDestroyed = true;

			OnDestroy();
		}
	}
	bool Component::IsDestroyed() const noexcept
	{
		return myIsDestroyed;
	}
	ComponentSystem* Component::GetSystem() const noexcept
	{
		return myActor.GetSystem();
	}
	ActorRef Component::GetActor() const noexcept
	{
		return myActor;
	}
	ComponentID Component::GetID() const noexcept
	{
		return myID;
	}

	void Component::Initialize(ComponentSystem* aSystem, ActorID aActor, ComponentID aID)
	{
		myActor = ActorRef(aSystem, aActor);
		myID = aID;

		OnInit();
	}
	void Component::Start()
	{
		OnStart();
	}
	void Component::Update()
	{
		OnUpdate();
	}

}
