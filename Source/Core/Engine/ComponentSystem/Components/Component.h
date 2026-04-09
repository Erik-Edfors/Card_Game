#pragma once
#include "../Reference.hpp"

class ComponentSystem;
class Actor;

class Component
{
	friend class ComponentSystem;
public:
	Component();
	virtual ~Component();

	void Destroy();
	bool IsDestroyed() const noexcept;

	ComponentSystem* GetSystem() const noexcept;
	ActorRef GetActor() const noexcept;
	ComponentID GetID() const noexcept;

protected:
	virtual void OnInit() {};
	virtual void OnStart() {};
	virtual void OnUpdate() {};
	virtual void OnDestroy() {};

private:
	void Initialize(ComponentSystem* aSystem, ActorID aActor, ComponentID aID);

	void Start();
	void Update();

	ComponentID myID;
	ActorRef myActor;
	bool myIsDestroyed;
};