#include "Engine.pch.h"
#include "SpriteComponent.h"
#include "TransformComponent.h"

SpriteComponent::SpriteComponent(SpriteRenderLayer* aLayer) :
	myRenderLayer(aLayer)
{
}
SpriteComponent::~SpriteComponent()
{
}

void SpriteComponent::SetAtlas(graphics::ShaderResource* aAtlas) noexcept
{
	mySprite.SetAtlas(aAtlas);
}
graphics::ShaderResource* SpriteComponent::GetAtlas() const noexcept
{
	return mySprite.GetAtlas();
}

void SpriteComponent::SetAtlasRect(CommonUtilities::AABB2Df aRect) noexcept
{
	mySprite.SetAtlasRect(aRect);
}
CommonUtilities::AABB2Df SpriteComponent::GetAtlasRect() const noexcept
{
	return mySprite.GetAtlasRect();
}

void SpriteComponent::SetTint(CommonUtilities::Vector4f aTint) noexcept
{
	mySprite.SetTint(aTint);
}
CommonUtilities::Vector4f SpriteComponent::GetTint() const noexcept
{
	return mySprite.GetTint();
}

void SpriteComponent::SetOrigin(CommonUtilities::Vector2f aOrigin) noexcept
{
	mySprite.SetOrigin(aOrigin);
}
CommonUtilities::Vector2f SpriteComponent::GetOrigin() const noexcept
{
	return mySprite.GetOrigin();
}

unsigned int SpriteComponent::GetID() const noexcept
{
	return mySprite.GetID();
}

bool SpriteComponent::IsHovered() const noexcept
{
	return myRenderLayer->GetHoveredID() == GetID();
}

SpriteRenderLayer* SpriteComponent::GetRenderLayer() const noexcept
{
	return myRenderLayer;
}

void SpriteComponent::OnStart()
{
	myTransform = GetSystem().GetComponent<TransformComponent>(GetEntityHandle());
}

void SpriteComponent::OnUpdate(float aDeltaTime)
{
	aDeltaTime;

	mySprite.SetTransform(myTransform->GetGlobalMatrix());

	myRenderLayer->Draw(mySprite);
}
