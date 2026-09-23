#pragma once
#include "Defines.h"
#include "ComponentRegistry.h"
#include <unordered_map>
#include <typeindex>
#include <memory>

namespace cs
{
	class Component;
	class Entity;

	class ComponentRegistryBase;

	class ComponentSystem
	{
	public:
		ComponentSystem();
		~ComponentSystem();

		void Init();
		void Shutdown();
		void Update(float aDeltaTime);

		EntityHandle CreateEntity();
		void Destroy(EntityHandle aEntityHandle);

		template<class _ComponentTy>
		bool RegisterComponentType();

		template<class _ComponentTy, class... _Args>
		_ComponentTy* AddComponent(EntityHandle aEntityHandle, _Args&&... aArgs);

		template<class _ComponentTy>
		_ComponentTy* GetComponent(ComponentHandle aHandle);

		template<class _ComponentTy>
		_ComponentTy* GetComponent(EntityHandle aEntityHandle);

		template<class _ComponentTy>
		ComponentIterator<_ComponentTy> Iterate(EntityHandle aHandle);

		template<class _ComponentTy>
		ComponentRegistry<_ComponentTy>* GetRegistry();

	private:
		EntityHandle GenerateEntityHandle() noexcept;
		ComponentHandle GenerateComponentHandle() noexcept;

		std::unordered_map<std::type_index, size_t> myRegistryIndex;
		std::vector<std::unique_ptr<ComponentRegistryBase>> myRegistries;

		EntityHandle myNextEntityHandle;
		ComponentHandle myNextComponentHandle;
	};

	template<class _ComponentTy>
	inline bool ComponentSystem::RegisterComponentType()
	{
		constexpr const std::type_info& componentTypeInfo = typeid(_ComponentTy);
		const std::type_index componentIndex(componentTypeInfo);

		if (not myRegistryIndex.contains(componentIndex))
		{
			const size_t index = myRegistries.size();
			myRegistries.push_back(std::make_unique<ComponentRegistry<_ComponentTy>>(this));
			myRegistryIndex[componentIndex] = index;
			return true;
		}

		return false;
	}

	template<class _ComponentTy, class... _Args>
	inline _ComponentTy* ComponentSystem::AddComponent(EntityHandle aEntityHandle, _Args&&... aArgs)
	{
		ComponentRegistry<_ComponentTy>* registry = GetRegistry<_ComponentTy>();
		if (registry != nullptr)
		{
			ComponentHandle handle = GenerateComponentHandle();
			return registry->AddComponent(handle, aEntityHandle, std::forward<_Args>(aArgs)...);
		}

		return nullptr;
	}

	template<class _ComponentTy>
	inline _ComponentTy* ComponentSystem::GetComponent(ComponentHandle aHandle)
	{
		ComponentRegistry<_ComponentTy>* registry = GetRegistry<_ComponentTy>();
		if (registry != nullptr)
		{
			return registry->GetComponent(aHandle);
		}

		return nullptr;
	}
	template<class _ComponentTy>
	inline _ComponentTy* ComponentSystem::GetComponent(EntityHandle aEntityHandle)
	{
		ComponentRegistry<_ComponentTy>* registry = GetRegistry<_ComponentTy>();
		if (registry != nullptr)
		{
			return registry->GetComponent(aEntityHandle);
		}

		return nullptr;
	}
	template<class _ComponentTy>
	inline ComponentIterator<_ComponentTy> ComponentSystem::Iterate(EntityHandle aHandle)
	{
		ComponentRegistry<_ComponentTy>* registry = GetRegistry<_ComponentTy>();
		if (registry != nullptr)
		{
			return registry->Iterate(aHandle);
		}

		return ComponentIterator<_ComponentTy>(nullptr, SIZE_MAX);
	}
	template<class _ComponentTy>
	inline ComponentRegistry<_ComponentTy>* ComponentSystem::GetRegistry()
	{
		constexpr const std::type_info& componentTypeInfo = typeid(_ComponentTy);
		const std::type_index componentIndex(componentTypeInfo);

		auto it = myRegistryIndex.find(componentIndex);
		if (it != myRegistryIndex.end())
		{
			return static_cast<ComponentRegistry<_ComponentTy>*>(myRegistries[it->second].get());
		}
		return nullptr;
	}
}