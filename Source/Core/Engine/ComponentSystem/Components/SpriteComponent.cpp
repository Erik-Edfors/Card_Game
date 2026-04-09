#include "Engine.pch.h"
#include "SpriteComponent.h"

#include "Engine.h"
#include "Scene.h"
#include "ComponentSystem/ComponentSystem.h"
#include "TransformComponent.h"

#include <Core/GraphicsEngine/GraphicsEngine.h>

SpriteComponent::SpriteComponent() : 
	myRenderLayer(nullptr),
	myPivot(0.5f, 0.5f)
{
}

SpriteComponent::~SpriteComponent()
{
}

void SpriteComponent::SetPivot(const CommonUtilities::Vector2f& aPivot) noexcept
{
	myPivot = aPivot;
}

void SpriteComponent::SetTextureRect(const CommonUtilities::AABB2Df& aTextureRect) noexcept
{
	myTextureRect = aTextureRect;
}

void SpriteComponent::OnInit()
{
	myRenderLayer = GetSystem().GetScene().GetEngine().GetGraphicsEngine().GetRenderLayer<SpriteRenderLayer>();
}

void SpriteComponent::OnStart()
{
	myTransform = GetActor()->GetComponent<TransformComponent>();
}

void SpriteComponent::OnUpdate()
{
	const Sprite sprite = {
		.Position = myTransform->GetTranslationGlobal().xy(),
		.Size = myTransform->GetScaleGlobal().xy(),
		.Pivot = myPivot,
		.Rotation = std::asinf(myTransform->GetRotationGlobal().z) * 2.f,

		.Tint = { 1.f, 1.f, 1.f, 1.f },
		.Roughness = 1.f,
		.Metallicity = 0.f,

		.TextureRect = myTextureRect,
		.Depth = myTransform->GetTranslationGlobal().z
	};
	myRenderLayer->Draw(sprite);
}
