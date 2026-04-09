#include "Engine.pch.h"
#include "Scene.h"
#include "ComponentSystem/ComponentSystem.h"

Scene::Scene() :
	myComponentSystem(nullptr),
	myEngine(nullptr)
{
}
void Scene::Initialize(Engine* aEngine)
{
	myEngine = aEngine;
	myComponentSystem = std::make_unique<ComponentSystem>(this);

	OnInit();
}
Scene::~Scene()
{
	myComponentSystem->DestroyAll();
}
void Scene::Update()
{
	myComponentSystem->Update();
}
ComponentSystem& Scene::GetComponentSystem() noexcept
{
	return *myComponentSystem;
}
Engine& Scene::GetEngine() noexcept
{
	return *myEngine;
}
