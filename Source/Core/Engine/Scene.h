#pragma once
#include <memory>
#include <Core/ComponentSystem/ComponentSystem.h>

class Engine;

//class ComponentSystem;

class Scene
{
	friend class Engine;
public:
	virtual ~Scene();

	void Update(float aDeltaTime);

	Engine& GetEngine() noexcept;
	cs::ComponentSystem& GetComponentSystem() noexcept;

protected:
	Scene();
	virtual void OnInit() {};
	virtual void OnUpdate(float aDeltaTime) { aDeltaTime; };

private:
	void Initialize(Engine* aEngine);

	Engine* myEngine;
	cs::ComponentSystem myComponentSystem;
};