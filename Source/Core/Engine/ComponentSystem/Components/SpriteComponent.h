#pragma once
#include "Component.h"
#include <Core/GraphicsEngine/RenderLayers/SpriteRenderLayer.h>
#include "../Reference.hpp"

class TransformComponent;

class SpriteComponent : public Component
{
public:
	SpriteComponent();
	~SpriteComponent();

	void SetPivot(const CommonUtilities::Vector2f& aPivot) noexcept;
	void SetTextureRect(const CommonUtilities::AABB2Df& aTextureRect) noexcept;

protected:
	void OnInit() override;
	void OnStart() override;
	void OnUpdate() override;
private:
	SpriteRenderLayer* myRenderLayer;
	ComponentRef<TransformComponent> myTransform;

	CommonUtilities::Vector2f myPivot;
	CommonUtilities::AABB2Df myTextureRect;
};