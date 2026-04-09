#include "Engine.pch.h"
#include "Reference.hpp"
#include "ComponentSystem.h"

ActorRef::ActorRef() noexcept :
	mySystem(nullptr),
	myActorID(INVALID_ACTOR)
{
}
ActorRef::ActorRef(std::nullptr_t) noexcept :
	mySystem(nullptr),
	myActorID(INVALID_ACTOR)
{
}
ActorRef::ActorRef(ComponentSystem* aSystem, ActorID aID) noexcept :
	mySystem(aSystem),
	myActorID(aID)
{
}
ActorRef::ActorRef(Actor* aActor) noexcept :
	mySystem(nullptr),
	myActorID(INVALID_ACTOR)
{
	if (aActor != nullptr)
	{
		mySystem = aActor->GetSystem();
		myActorID = aActor->GetID();
	}
}
ActorRef::~ActorRef() noexcept
{
}
Actor* ActorRef::Get() const noexcept
{
	if (mySystem != nullptr)
	{
		return mySystem->FindActor(myActorID);
	}
	return nullptr;
}
ComponentSystem* ActorRef::GetSystem() const noexcept
{
	return mySystem;
}
bool ActorRef::Expired() const noexcept
{
	if (mySystem != nullptr)
	{
		return mySystem->FindActor(myActorID) != nullptr;
	}
	return false;
}

Actor* ActorRef::operator->() const noexcept
{
	return Get();
}
Actor& ActorRef::operator*() const
{
	return *Get();
}
ActorRef::operator bool() const noexcept
{
	return not Expired();
}
