#pragma once
#include "Actor.h"

#include <vector>
#include <map>
#include <unordered_map>
#include <unordered_set>

namespace shared
{
	class ComponentSystem
	{
	public:
		ComponentSystem();
		~ComponentSystem();

		void Update();

		void DestroyAll();

		Actor* CreateActor() noexcept;

		template<class _Ty>
		_Ty* AddComponent(ActorID aActor) noexcept;

		Actor* FindActor(ActorID aID) noexcept;
		Component* FindComponent(ComponentID aID) noexcept;

	private:
		std::map<ActorID, Actor> myActors;
		ActorID myNextActorID;

		std::map<ComponentID, Component*> myComponents;
		ComponentID myNextComponentID;

		std::unordered_set<ComponentID> myNewComponents;
	};
	template<class _Ty>
	inline _Ty* ComponentSystem::AddComponent(ActorID aActor) noexcept
	{
		const ComponentID id = myNextComponentID++;
		myNewComponents.insert(id);

		_Ty* component = new _Ty();
		myComponents[id] = component;
		aActor->InsertComponent(id);

		component->Initialize(this, aActor, id);
		
		return component;
	}
}