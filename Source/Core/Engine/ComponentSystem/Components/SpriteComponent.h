#pragma once
#include "Component.h"
#include "../Reference.hpp"
#include <Core/CommonUtilities/Math/Vector2.hpp>
#include <Core/CommonUtilities/Math/AABB2D.hpp>

class TransformComponent;
class SpriteRenderLayer;

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