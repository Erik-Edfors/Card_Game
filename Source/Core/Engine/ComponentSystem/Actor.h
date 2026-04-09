#pragma once
#include "Defines.h"
#include "Components/Component.h"

#include <vector>

class ComponentSystem;

class Actor
{
	friend class ComponentSystem;
public:
	Actor();
	~Actor();

	void Destroy();
	bool IsDestroyed() const noexcept;

	ActorID GetID() const noexcept;
	ComponentSystem* GetSystem() const noexcept;

	const std::vector<ComponentRef<Component>>& GetComponents() noexcept;

	template<class _ComponentTy>
	ComponentRef<_ComponentTy> GetComponent();

private:
	void Initialize(ComponentSystem* aSystem, ActorID aID);

	void InsertComponent(ComponentID aComponent);

	std::vector<ComponentRef<Component>> myComponents;

	ActorID myID;
	ComponentSystem* mySystem;
	bool myIsDestroyed;
};

template<class _ComponentTy>
inline ComponentRef<_ComponentTy> Actor::GetComponent()
{
	for (const ComponentRef<Component>& component : myComponents)
	{
		_ComponentTy* result = dynamic_cast<_ComponentTy*>(component.Get());
		if (result != nullptr)
		{
			return ComponentRef<_ComponentTy>(result);
		}
	}
	return ComponentRef<_ComponentTy>(nullptr);
}