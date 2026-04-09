#include "Engine.pch.h"
#include "ComponentSystem.h"
#include "Scene.h"

ComponentSystem::ComponentSystem(Scene* aScene) :
	myScene(aScene),
	myNextActorID(1u),
	myNextComponentID(1u)
{
}
ComponentSystem::~ComponentSystem()
{
}
void ComponentSystem::Update()
{
	decltype(myComponents) stagedComponents;
	decltype(myNewComponents) stagedNewComponent;

	std::erase_if(
		myComponents,
		[](const decltype(myComponents)::value_type& aEntry) {
			return aEntry.second->IsDestroyed();
		}
	);

	stagedComponents = myComponents;
	stagedNewComponent = std::move(myNewComponents);
	myNewComponents = {};

	for (auto& entry : stagedComponents)
	{
		const ComponentID id = entry.first;
		Component* const component = entry.second;

		if (stagedNewComponent.contains(id))
		{
			component->Start();
		}
		component->Update();
	}
}
void ComponentSystem::DestroyAll()
{
	decltype(myComponents) stagedComponents;
	stagedComponents = myComponents;

	for (auto& entry : stagedComponents)
	{
		Component* const component = entry.second;

		component->Destroy();
	}
}
Actor* ComponentSystem::CreateActor() noexcept
{
	const ActorID id = myNextActorID++;

	const auto& entry = myActors.emplace(id, Actor()).first;
	Actor& actor = entry->second;

	actor.Initialize(this, id);

	return &actor;
}
Actor* ComponentSystem::FindActor(ActorID aID) noexcept
{
	const auto& entry = myActors.find(aID);
	if (entry != myActors.end())
	{
		return &entry->second;
	}
	return nullptr;
}
Component* ComponentSystem::FindComponent(ComponentID aID) noexcept
{
	const auto& entry = myComponents.find(aID);
	if (entry != myComponents.end())
	{
		return entry->second;
	}
	return nullptr;
}
Scene& ComponentSystem::GetScene() noexcept
{
	return *myScene;
}
