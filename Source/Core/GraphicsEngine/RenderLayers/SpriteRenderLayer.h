#pragma once
#include "RenderLayer.h"
#include "../Objects/PipelineState.h"
#include <Core/CommonUtilities/Math/Matrix4x4.hpp>
#include <Core/CommonUtilities/Math/Vector2.hpp>
#include <Core/CommonUtilities/Math/AABB2D.hpp>

namespace graphics
{
	class ShaderResource;
	class RenderTarget;
	class BlendState;

	class ConstantBuffer;
	class VertexBuffer;

	class VertexShader;
	class GeometryShader;
	class PixelShader;
}

struct Sprite
{
	CommonUtilities::Vector2f Position;
	CommonUtilities::Vector2f Size;
	CommonUtilities::Vector2f Pivot;
	float Rotation;

	CommonUtilities::Vector4f Tint;
	float Roughness;
	float Metallicity;

	graphics::PImpl<graphics::ShaderResource> Texture;
	CommonUtilities::AABB2Df TextureRect;

	float Depth;
};

class SpriteRenderLayer : public RenderLayer
{
public:
	SpriteRenderLayer();
	~SpriteRenderLayer();

	void SetTarget(graphics::RenderTarget* aRenderTarget) noexcept;
	void SetAtlas(graphics::ShaderResource* aAtlas) noexcept;

	void SetView(const CommonUtilities::Matrix4x4f& aView) noexcept;
	void SetProjection(const CommonUtilities::Matrix4x4f& aProjection) noexcept;

	void Draw(const Sprite& aSprite);

protected:
	void OnInit() override;
	void OnRender() override;

private:
	PipelineStateDesc myPipelineState;

	CommonUtilities::Matrix4x4f myView;
	CommonUtilities::Matrix4x4f myProjection;

	std::vector<Sprite> mySprites;

	graphics::PImpl<graphics::VertexBuffer> myVertexBuffer;

	graphics::PImpl<graphics::VertexShader> myVertexShader;
	graphics::PImpl<graphics::GeometryShader> myGeometryShader;
	graphics::PImpl<graphics::PixelShader> myPixelShader;

	graphics::PImpl<graphics::InputLayout> myInputLayout;

	graphics::PImpl<graphics::ConstantBuffer> myFrameBuffer;
	graphics::PImpl<graphics::ConstantBuffer> myMaterialBuffer;

	graphics::PImpl<graphics::BlendState> myBlendState;

	graphics::PImpl<graphics::RenderTarget> myRenderTarget;
	graphics::PImpl<graphics::ShaderResource> myTextureAtlas;
};