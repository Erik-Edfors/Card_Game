#pragma once
#include <memory>

namespace shared
{
	class ComponentSystem;

	class Scene
	{
	public:
		Scene();
		virtual ~Scene();

		void Update();

		ComponentSystem& GetComponentSystem();

	protected:
		virtual void OnUpdate() {};

	private:
		std::unique_ptr<ComponentSystem> myComponentSystem;
	};
}