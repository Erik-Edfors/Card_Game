#pragma once
#include "Defines.h"

namespace cs
{
	class ComponentSystem;
	class ComponentRegistryBase;

	class ComponentControlls
	{
		template<class _ComponentTy>
		friend class ComponentRegistry;
	private:
		virtual void Initialize(ComponentRegistryBase* aRegistry, ComponentHandle aHandle, EntityHandle aEntityHandle) = 0;
		virtual void Start() = 0;
		virtual void Update(float aDeltaTime) = 0;
		virtual void LateUpdate(float aDeltaTime) = 0;
	};

	class Component : public ComponentControlls
	{
	public:
		Component();

		void SetEnabled(bool aEnabled) noexcept;
		bool IsEnabled() const noexcept;

		void Destroy();
		bool IsDestroyed() const noexcept;
		bool IsStarted() const noexcept;

		ComponentRegistryBase& GetRegistry() const noexcept;
		ComponentSystem& GetSystem() const noexcept;
		ComponentHandle GetHandle() const noexcept;
		EntityHandle GetEntityHandle() const noexcept;

	protected:
		virtual void OnInit() {};
		virtual void OnStart() {};
		virtual void OnEnable() {};
		virtual void OnDisable() {};
		virtual void OnUpdate(float aDeltaTime) { aDeltaTime; };
		virtual void OnLateUpdate(float aDeltaTime) { aDeltaTime; };
		virtual void OnDestroy() {};

	private:
		virtual void Initialize(ComponentRegistryBase* aRegistry, ComponentHandle aHandle, EntityHandle aEntityHandle) override;
		virtual void Start() override;
		virtual void Update(float aDeltaTime) override;
		virtual void LateUpdate(float aDeltaTime) override;

		ComponentRegistryBase* myRegistry;
		ComponentHandle myHandle;
		EntityHandle myEntityHandle;
		bool myEnabled;
		bool myIsDestroyed;
		bool myIsStarted;
	};
}