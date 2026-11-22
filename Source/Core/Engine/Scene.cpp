#include "Engine.pch.h"
#include "Scene.h"
#include "Engine.h"

Scene::Scene() :
	myEngine(nullptr),
	myStarted(false)
{
}
Scene::~Scene()
{
}
void Scene::Update(float aDeltaTime)
{
	if (not myStarted)
	{
		OnStart();
		myStarted = true;
	}
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

bool Scene::HasStarted() const noexcept
{
	return myStarted;
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
