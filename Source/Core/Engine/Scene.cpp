#include "Engine.pch.h"
#include "Scene.h"
#include "Engine.h"

Scene::Scene() :
	myEngine(nullptr)
{
}
void Scene::Initialize(Engine* aEngine)
{
	myEngine = aEngine;

	OnInit();
}
Scene::~Scene()
{
}
void Scene::Update(float aDeltaTime)
{
	//TODO: Actually get delta time!
	GetComponentSystem().Update(aDeltaTime);
	OnUpdate(aDeltaTime);
}
Engine& Scene::GetEngine() noexcept
{
	return *myEngine;
}
cs::ComponentSystem& Scene::GetComponentSystem() noexcept
{
	return myComponentSystem;
}
