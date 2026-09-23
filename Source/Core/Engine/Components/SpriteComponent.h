#pragma once
#include <Core/ComponentSystem/Component.h>
#include <Core/ComponentSystem/ComponentRef.hpp>

#include <Core/GraphicsEngine/RenderLayers/SpriteRenderLayer.h>

#include <Core/CommonUtilities/Math/AABB2D.hpp>
#include <Core/CommonUtilities/Math/Vector.h>

namespace graphics
{
	class ShaderResource;
}

class TransformComponent;

class SpriteComponent : public cs::Component
{
public:
	SpriteComponent(SpriteRenderLayer* aLayer);
	~SpriteComponent();

	void SetAtlas(graphics::ShaderResource* aAtlas) noexcept;
	graphics::ShaderResource* GetAtlas() const noexcept;

	void SetAtlasRect(CommonUtilities::AABB2Df aRect) noexcept;
	CommonUtilities::AABB2Df GetAtlasRect() const noexcept;

	void SetTint(CommonUtilities::Vector4f aTint) noexcept;
	CommonUtilities::Vector4f GetTint() const noexcept;

	void SetOrigin(CommonUtilities::Vector2f aOrigin) noexcept;
	CommonUtilities::Vector2f GetOrigin() const noexcept;

protected:
	virtual void OnStart() override;
	virtual void OnUpdate(float aDeltaTime) override;

private:
	cs::ComponentRef<TransformComponent> myTransform;

	Sprite mySprite;
	SpriteRenderLayer* myRenderLayer;
};