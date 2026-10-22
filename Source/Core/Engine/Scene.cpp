#include "Engine.pch.h"
#include "Scene.h"
#include "Engine.h"

Scene::Scene() :
	myEngine(nullptr)
{
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
void Scene::Initialize(Engine* aEngine)
{
	myEngine = aEngine;

	OnInit();
}
void Scene::Shutdown()
{
	OnShutdown();
}
void Scene::Enter()
{
	OnEnter();
}
void Scene::Exit()
{
	OnExit();
}
