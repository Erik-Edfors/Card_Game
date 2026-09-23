#pragma once
#include "Defines.h"
#include "Component.h"
#include <vector>
#include <unordered_map>

namespace cs
{
	class ComponentSystem;

	template<class _ComponentTy>
	class ComponentIterator;

	class ComponentRegistryBase
	{
	public:
		virtual void Destroy(EntityHandle aEntityHandle) = 0;

		virtual void Start() = 0;
		virtual void Update(float aDeltaTime) = 0;
		virtual void LateUpdate(float aDeltaTime) = 0;
		virtual void Cleanup() = 0;
		virtual void Shutdown() = 0;
	private:
	};

	template<class _ComponentTy>
	class ComponentRegistry : public ComponentRegistryBase
	{
		friend class ComponentIterator<_ComponentTy>;
	public:
		ComponentRegistry(ComponentSystem* aSystem);
		~ComponentRegistry();

		template<class... _Args>
		_ComponentTy* AddComponent(ComponentHandle aHandle, EntityHandle aEntityHandle, _Args&&... aArgs);

		_ComponentTy* GetComponent(ComponentHandle aHandle);
		_ComponentTy* GetComponent(EntityHandle aHandle);
		ComponentIterator<_ComponentTy> Iterate(EntityHandle aHandle);

		virtual void Destroy(EntityHandle aEntityHandle) override;

		virtual void Start() override;
		virtual void Update(float aDeltaTime) override;
		virtual void LateUpdate(float aDeltaTime) override;
		virtual void Cleanup() override;
		virtual void Shutdown() override;

		ComponentSystem& GetSystem() noexcept;

	private:
		struct _ListEntry
		{
			size_t Previous;
			size_t Next;
			_ComponentTy Component;
		};

		_ListEntry& AddEntry(_ComponentTy&& aComponent, ComponentHandle aHandle, EntityHandle aEntityHandle);
		void RemoveEntry(ComponentHandle aHandle);

		ComponentSystem* mySystem;
		std::vector<_ListEntry> myList;
		std::unordered_map<ComponentHandle, size_t> myHandleIndexMap;
		std::unordered_map<EntityHandle, size_t> myEntityIndexMap;
	};

	template<class _ComponentTy>
	class ComponentIterator
	{
	public:
		using iterator_category = std::forward_iterator_tag;
		using value_type = _ComponentTy;
		using difference_type = ptrdiff_t;
		using pointer = _ComponentTy*;
		using reference = _ComponentTy&;

		ComponentIterator(ComponentRegistry<_ComponentTy>* aRegistry, size_t aIndex) :
			myRegistry(aRegistry),
			myIndex(aIndex)
		{
		};

		// Dereference operators
		reference operator*() const { return myRegistry->myList[myIndex].Component; }
		pointer operator->() const noexcept { return &myRegistry->myList[myIndex]; }

		// Increment and decrement operators
		ComponentIterator& operator++() noexcept
		{
			if (myIndex != SIZE_MAX)
			{
				myIndex = myRegistry->myList[myIndex].Next;
			}
			return *this;
		}
		ComponentIterator& operator--() noexcept
		{
			if (myIndex != SIZE_MAX)
			{
				myIndex = myRegistry->myList[myIndex].Previous;
			}
			return *this;
		}
		ComponentIterator operator++(int) noexcept { ComponentIterator temp = *this; ++(*this); return temp; }
		ComponentIterator operator--(int) noexcept { ComponentIterator temp = *this; --(*this); return temp; }

		// Arithmetic operators
		ComponentIterator operator+(difference_type n) const noexcept
		{
			size_t index = myIndex;
			for (difference_type i = 0; i < n && index != SIZE_MAX; i++)
			{
				index = myRegistry->myList[index].Next;
			}
			return ComponentIterator(myRegistry, index);
		}
		ComponentIterator operator-(difference_type n) const noexcept
		{
			size_t index = myIndex;
			for (difference_type i = 0; i < n && index != SIZE_MAX; i++)
			{
				index = myRegistry->myList[index].Previous;
			}
			return ComponentIterator(myRegistry, index);
		}
		//difference_type operator-(const ComponentIterator& other) const noexcept { return myPtr - other.myPtr; }

		// Comparison operators
		bool operator==(const ComponentIterator& other) const noexcept { return myIndex == other.myIndex && myRegistry == other.myRegistry; }
		bool operator!=(const ComponentIterator& other) const noexcept { return myIndex != other.myIndex || myRegistry != other.myRegistry; }
		bool operator<(const ComponentIterator& other) const noexcept { return myIndex < other.myIndex; }
		bool operator>(const ComponentIterator& other) const noexcept { return myIndex > other.myIndex; }
		bool operator<=(const ComponentIterator& other) const noexcept { return myIndex <= other.myIndex; }
		bool operator>=(const ComponentIterator& other) const noexcept { return myIndex >= other.myIndex; }

		ComponentIterator begin() { return *this; }
		ComponentIterator end() { return ComponentIterator(myRegistry, SIZE_MAX); }

	private:
		ComponentRegistry<_ComponentTy>* myRegistry;
		size_t myIndex;
	};

	template<class _ComponentTy>
	template<class ..._Args>
	inline _ComponentTy* ComponentRegistry<_ComponentTy>::AddComponent(ComponentHandle aHandle, EntityHandle aEntityHandle, _Args&&... aArgs)
	{
		_ComponentTy component = _ComponentTy(std::forward<_Args>(aArgs)...);
		return &AddEntry(std::move(component), aHandle, aEntityHandle).Component;
	}
	template<class _ComponentTy>
	inline ComponentRegistry<_ComponentTy>::ComponentRegistry(ComponentSystem* aSystem) :
		mySystem(aSystem)
	{
	}
	template<class _ComponentTy>
	inline ComponentRegistry<_ComponentTy>::~ComponentRegistry()
	{
		Shutdown();
	}
	template<class _ComponentTy>
	inline _ComponentTy* ComponentRegistry<_ComponentTy>::GetComponent(ComponentHandle aHandle)
	{
		auto it = myHandleIndexMap.find(aHandle);
		if (it == myHandleIndexMap.end())
		{
			return nullptr;
		}
		return &myList[it->second].Component;
	}
	template<class _ComponentTy>
	inline _ComponentTy* ComponentRegistry<_ComponentTy>::GetComponent(EntityHandle aHandle)
	{
		auto it = myEntityIndexMap.find(aHandle);
		if (it == myEntityIndexMap.end())
		{
			return nullptr;
		}
		return &myList[it->second].Component;
	}
	template<class _ComponentTy>
	inline ComponentIterator<_ComponentTy> ComponentRegistry<_ComponentTy>::Iterate(EntityHandle aHandle)
	{
		auto it = myEntityIndexMap.find(aHandle);
		if (it == myEntityIndexMap.end())
		{
			return ComponentIterator<_ComponentTy>(this, SIZE_MAX);
		}
		return ComponentIterator<_ComponentTy>(this, it->second);
	}

	template<class _ComponentTy>
	inline void ComponentRegistry<_ComponentTy>::Destroy(EntityHandle aEntityHandle)
	{
		for (_ComponentTy& component : Iterate(aEntityHandle))
		{
			component.Destroy();
		}
	}

	template<class _ComponentTy>
	inline void ComponentRegistry<_ComponentTy>::Start()
	{
		for (size_t i = 0; i < myList.size(); i++)
		{
			_ListEntry& entry = myList[i];
			static_cast<ComponentControlls&>(entry.Component).Start();
		}
	}

	template<class _ComponentTy>
	inline void ComponentRegistry<_ComponentTy>::Update(float aDeltaTime)
	{
		for (size_t i = 0; i < myList.size(); i++)
		{
			_ListEntry& entry = myList[i];
			if (entry.Component.IsStarted())
			{
				static_cast<ComponentControlls&>(entry.Component).Update(aDeltaTime);
			}
		}
	}

	template<class _ComponentTy>
	inline void ComponentRegistry<_ComponentTy>::LateUpdate(float aDeltaTime)
	{
		for (size_t i = 0; i < myList.size(); i++)
		{
			_ListEntry& entry = myList[i];
			if (entry.Component.IsStarted())
			{
				static_cast<ComponentControlls&>(entry.Component).LateUpdate(aDeltaTime);
			}
		}
	}

	template<class _ComponentTy>
	inline void ComponentRegistry<_ComponentTy>::Cleanup()
	{
		size_t i = 0;
		while (i < myList.size())
		{
			_ListEntry& entry = myList[i];
			if (entry.Component.IsDestroyed())
			{
				RemoveEntry(entry.Component.GetHandle());
			}
			else
			{
				++i;
			}
		}
	}

	template<class _ComponentTy>
	inline void ComponentRegistry<_ComponentTy>::Shutdown()
	{
		for (size_t i = 0; i < myList.size(); i++)
		{
			_ListEntry& entry = myList[i];
			entry.Component.Destroy();
		}
		Cleanup();
	}

	template<class _ComponentTy>
	inline ComponentSystem& ComponentRegistry<_ComponentTy>::GetSystem() noexcept
	{
		return *mySystem;
	}


	template<class _ComponentTy>
	inline ComponentRegistry<_ComponentTy>::_ListEntry& ComponentRegistry<_ComponentTy>::AddEntry(_ComponentTy&& aComponent, ComponentHandle aHandle, EntityHandle aEntityHandle)
	{
		_ListEntry entry = _ListEntry{
			.Previous = SIZE_MAX,
			.Next = SIZE_MAX,
			.Component = std::move(aComponent)
		};

		const size_t index = myList.size();

		myHandleIndexMap[aHandle] = index;

		{
			auto it = myEntityIndexMap.find(aEntityHandle);
			if (it != myEntityIndexMap.end())
			{
				const size_t nextIndex = it->second;
				_ListEntry& nextEntry = myList[nextIndex];

				entry.Next = nextIndex;
				nextEntry.Previous = index;
				it->second = index;
			}
			else
			{
				myEntityIndexMap[aEntityHandle] = index;
			}
		}

		myList.push_back(std::move(entry));
		static_cast<ComponentControlls&>(myList.back().Component).Initialize(&GetSystem(), aHandle, aEntityHandle);
		return myList.back();
	}
	template<class _ComponentTy>
	inline void ComponentRegistry<_ComponentTy>::RemoveEntry(ComponentHandle aHandle)
	{
		size_t index;
		{
			auto it = myHandleIndexMap.find(aHandle);
			if (it == myHandleIndexMap.end())
			{
				return;	// Return if handle has no associated component in this registry.
			}
			index = it->second;
			myHandleIndexMap.erase(it);
		}

		const _ListEntry& entry = myList[index];
		const EntityHandle entity = entry.Component.GetEntityHandle();

		if (entry.Next == SIZE_MAX && entry.Previous == SIZE_MAX)	// If no previous or next entry exists, then no entry for the given entity will exist after its removal.
		{
			myEntityIndexMap.erase(entity);
		}
		else
		{
			// Remove entry from linked list.
			if (entry.Next != SIZE_MAX)
			{
				myList[entry.Next].Previous = entry.Previous;
			}
			if (entry.Previous != SIZE_MAX)
			{
				myList[entry.Previous].Next = entry.Next;
			}
			else	// If no previous entry exists, then entity index map is pointing to this entry, and needs updating.
			{
				myEntityIndexMap[entity] = entry.Next;
			}
		}

		if (index == myList.size() - 1)	// if entry is last element in list, no moving of entries required.
		{
			myList.pop_back();
			return;
		}

		// The entry that will be moved to index;
		_ListEntry& back = myList.back();
		const ComponentHandle backHandle = back.Component.GetHandle();

		if (back.Next != SIZE_MAX)
		{
			myList[back.Next].Previous = index;
		}
		if (back.Previous != SIZE_MAX)
		{
			myList[back.Previous].Next = index;
		}
		else
		{
			const EntityHandle backEntity = back.Component.GetEntityHandle();
			myEntityIndexMap[backEntity] = index;
		}

		myHandleIndexMap[backHandle] = index;
		myList[index] = std::move(myList.back());
		myList.pop_back();
	}
}