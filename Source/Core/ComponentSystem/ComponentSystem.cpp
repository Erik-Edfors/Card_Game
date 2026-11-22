#include "ComponentSystem.pch.h"
#include "ComponentSystem.h"

namespace cs
{
	ComponentSystem::ComponentSystem() : 
		myNextEntityHandle(INVALID_ENTITY_HANDLE.Handle + 1),
		myNextComponentHandle(INVALID_COMPONENT_HANDLE.Handle + 1)
	{
	}
	ComponentSystem::~ComponentSystem()
	{
		Shutdown();
	}

	void ComponentSystem::Init()
	{

	}
	void ComponentSystem::Shutdown()
	{
		for (const std::unique_ptr<ComponentRegistryBase>& registry : myRegistries)
		{
			registry->Shutdown();
		}
		myRegistries.clear();
		myRegistryIndex.clear();
	}
	void ComponentSystem::Update(float aDeltaTime)
	{
		for (const std::unique_ptr<ComponentRegistryBase>& registry : myRegistries)
		{
			registry->Cleanup();
		} 

		for (const std::unique_ptr<ComponentRegistryBase>& registry : myRegistries)
		{
			registry->Start();
		}

		for (const std::unique_ptr<ComponentRegistryBase>& registry : myRegistries)
		{
			registry->Update(aDeltaTime);
		}

		for (const std::unique_ptr<ComponentRegistryBase>& registry : myRegistries)
		{
			registry->LateUpdate(aDeltaTime);
		}
	}
	EntityHandle ComponentSystem::CreateEntity()
	{
		return GenerateEntityHandle();
	}
	void ComponentSystem::Destroy(EntityHandle aEntityHandle)
	{
		for (const std::unique_ptr<ComponentRegistryBase>& registry : myRegistries)
		{
			registry->Destroy(aEntityHandle);
		}
	}
	EntityHandle ComponentSystem::GenerateEntityHandle() noexcept
	{
		EntityHandle result = myNextEntityHandle;
		++myNextEntityHandle.Handle;
		return result;
	}
	ComponentHandle ComponentSystem::GenerateComponentHandle() noexcept
	{
		ComponentHandle result = myNextComponentHandle;
		++myNextComponentHandle.Handle;
		return result;
	}
}