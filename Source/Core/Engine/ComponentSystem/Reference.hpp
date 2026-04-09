#pragma once
#include "Defines.h"
#include <type_traits>

class ComponentSystem;
class Actor;

class ActorRef
{
public:
	ActorRef() noexcept;
	ActorRef(std::nullptr_t) noexcept;
	ActorRef(ComponentSystem* aSystem, ActorID aID) noexcept;
	ActorRef(Actor* aActor) noexcept;
	~ActorRef() noexcept;

	Actor* Get() const noexcept;
	ComponentSystem* GetSystem() const noexcept;
	bool Expired() const noexcept;

	Actor* operator->() const noexcept;
	Actor& operator*() const;

	operator bool() const noexcept;
	operator ActorID() const noexcept;

private:
	ComponentSystem* mySystem;
	ActorID myActorID;
};

template<class _Ty>
class ComponentRef
{
public:
	ComponentRef() noexcept;
	ComponentRef(std::nullptr_t) noexcept;
	ComponentRef(ComponentSystem* aSystem, ComponentID aID) noexcept;
	ComponentRef(_Ty* aComponent) noexcept;
	~ComponentRef() noexcept;

	template<class _OtherTy>
		requires(std::is_base_of_v<_Ty, _OtherTy>)
	ComponentRef(_OtherTy* aComponent) noexcept;

	_Ty* Get() const noexcept;
	ComponentSystem* GetSystem() const noexcept;
	bool Expired() const noexcept;

	_Ty* operator->() const noexcept;
	_Ty& operator*() const;

	operator bool() const noexcept;

private:
	ComponentSystem* mySystem;
	ComponentID myComponentID;
};



template<class _Ty>
ComponentRef<_Ty>::ComponentRef() noexcept :
	mySystem(nullptr),
	myComponentID(INVALID_COMPONENT)
{
}
template<class _Ty>
ComponentRef<_Ty>::ComponentRef(std::nullptr_t) noexcept :
	mySystem(nullptr),
	myComponentID(INVALID_COMPONENT)
{
}
template<class _Ty>
inline ComponentRef<_Ty>::ComponentRef(ComponentSystem* aSystem, ComponentID aID) noexcept :
	mySystem(aSystem),
	myComponentID(aID)
{
}
template<class _Ty>
ComponentRef<_Ty>::ComponentRef(_Ty* aComponent) noexcept :
	mySystem(nullptr),
	myComponentID(INVALID_COMPONENT)
{
	if (aComponent != nullptr)
	{
		mySystem = &aComponent->GetSystem();
		myComponentID = aComponent->GetID();
	}
}
template<class _Ty>
ComponentRef<_Ty>::~ComponentRef() noexcept
{
}
template<class _Ty>
template<class _OtherTy>
	requires(std::is_base_of_v<_Ty, _OtherTy>)
inline ComponentRef<_Ty>::ComponentRef(_OtherTy* aComponent) noexcept :
	mySystem(nullptr),
	myComponentID(INVALID_COMPONENT)
{
	if (aComponent != nullptr)
	{
		mySystem = aComponent->GetSystem();
		myComponentID = aComponent->GetID();
	}
}

template<class _Ty>
inline _Ty* ComponentRef<_Ty>::Get() const noexcept
{
	if (mySystem != nullptr)
	{
		return static_cast<_Ty*>(mySystem->FindComponent(myComponentID));
	}
	return nullptr;
}
template<class _Ty>
inline ComponentSystem* ComponentRef<_Ty>::GetSystem() const noexcept
{
	return mySystem;
}

template<class _Ty>
inline bool ComponentRef<_Ty>::Expired() const noexcept
{
	if (mySystem != nullptr)
	{
		return mySystem->FindComponent(myComponentID) != nullptr;
	}
	return false;
}

template<class _Ty>
inline _Ty* ComponentRef<_Ty>::operator->() const noexcept
{
	return Get();
}
template<class _Ty>
inline _Ty& ComponentRef<_Ty>::operator*() const
{
	return *Get();
}

template<class _Ty>
ComponentRef<_Ty>::operator bool() const noexcept
{
	return not Expired();
}