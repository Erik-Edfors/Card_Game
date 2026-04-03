#include "Shared.pch.h"
#include "Component.h"

namespace shared
{
	Component::Component() :
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

	void Component::Initialize(ComponentSystem* aSystem, ActorID aActor)
	{
		myActor = ActorRef(aSystem, aActor);

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
