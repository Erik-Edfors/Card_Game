#pragma once
#include "../Reference.hpp"

namespace shared
{
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

	protected:
		virtual void OnInit() {};
		virtual void OnStart() {};
		virtual void OnUpdate() {};
		virtual void OnDestroy() {};

	private:
		void Initialize(ComponentSystem* aSystem, ActorID aActor);

		void Start();
		void Update();

		ActorRef myActor;
		bool myIsDestroyed;
	};
}