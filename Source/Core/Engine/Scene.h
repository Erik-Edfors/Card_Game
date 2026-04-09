#pragma once
#include <memory>

class Engine;

class ComponentSystem;

class Scene
{
	friend class Engine;
public:
	virtual ~Scene();

	void Update();

	ComponentSystem& GetComponentSystem() noexcept;
	Engine& GetEngine() noexcept;

protected:
	Scene();
	virtual void OnInit() {};
	virtual void OnUpdate() {};

private:
	void Initialize(Engine* aEngine);

	std::unique_ptr<ComponentSystem> myComponentSystem;
	Engine* myEngine;
};