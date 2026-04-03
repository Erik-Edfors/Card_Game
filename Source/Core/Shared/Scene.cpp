#include "Shared.pch.h"
#include "Scene.h"
#include "ComponentSystem/ComponentSystem.h"

namespace shared
{
	Scene::Scene() : 
		myComponentSystem(nullptr)
	{
		myComponentSystem = std::make_unique<ComponentSystem>();
	}
	Scene::~Scene()
	{
		myComponentSystem->DestroyAll();
	}
	void Scene::Update()
	{
		myComponentSystem->Update();
	}
	ComponentSystem& Scene::GetComponentSystem()
	{
		return *myComponentSystem;
	}
}
