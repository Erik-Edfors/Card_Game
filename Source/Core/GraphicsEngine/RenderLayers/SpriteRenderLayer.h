#pragma once
#include "RenderLayer.h"

#include <Core/CommonUtilities/Math/AABB2D.hpp>
#include <Core/CommonUtilities/Math/Matrix4x4.hpp>

#include <Core/RenderHardwareInterface/PImpl.hpp>
#include <Core/RenderHardwareInterface/Objects/ShaderResource.h>
#include <Core/RenderHardwareInterface/Objects/Buffers/ConstantBuffer.h>

#include <unordered_set>

namespace graphics
{
	class ShaderResource;
	class RenderTarget;
	class DepthStencil;
	class SamplerState;

	class VertexBuffer;
	class ConstantBuffer;

	class BlendState;
	class InputLayout;

	class VertexShader;
	class GeometryShader;
	class PixelShader;
}

namespace assets
{
	class ShaderRegistry;
}

class Sprite
{
public:
	struct Instance
	{
		graphics::PImpl<graphics::ConstantBuffer> ObjectBuffer;
		graphics::PImpl<graphics::ShaderResource> Atlas;
	};

	Sprite();
	~Sprite();

	void SetAtlas(graphics::ShaderResource* aAtlas) noexcept;
	graphics::ShaderResource* GetAtlas() const noexcept;

	void SetAtlasRect(CommonUtilities::AABB2Df aRect) noexcept;
	CommonUtilities::AABB2Df GetAtlasRect() const noexcept;

	void SetTint(CommonUtilities::Vector4f aTint) noexcept;
	CommonUtilities::Vector4f GetTint() const noexcept;

	void SetOrigin(CommonUtilities::Vector2f aOrigin) noexcept;
	CommonUtilities::Vector2f GetOrigin() const noexcept;

	void SetTransform(const CommonUtilities::Matrix4x4f& aTransform) noexcept;
	const CommonUtilities::Matrix4x4f& GetTransform() const noexcept;

	unsigned int GetID() const noexcept;

	Instance GetInstance(graphics::Device* aDevice) const;

private:
	void Clean(graphics::Device* aDevice) const;

	mutable bool myObjectBufferIsDirty;
	mutable Instance myInstance;

	CommonUtilities::Matrix4x4f myTransform;
	CommonUtilities::Vector4f myTint;
	CommonUtilities::AABB2Df myAtlasRect;
	CommonUtilities::Vector2f myOrigin;
	unsigned int myID;
};

class SpriteRenderLayer : public RenderLayer
{
public:
	SpriteRenderLayer(assets::ShaderRegistry* aShaderRegistry);
	~SpriteRenderLayer();

	void SetView(const CommonUtilities::Matrix4x4f& aView) noexcept;
	const CommonUtilities::Matrix4x4f& GetView() const noexcept;

	void SetProjection(const CommonUtilities::Matrix4x4f& aProjection);
	const CommonUtilities::Matrix4x4f& GetProjection() const noexcept;

	void SetRenderTarget(graphics::RenderTarget* aRenderTarget) noexcept;
	graphics::RenderTarget* GetRenderTarget() const noexcept;

	void SetDepthStencil(graphics::DepthStencil* aDepthStencil) noexcept;
	graphics::DepthStencil* GetDepthStencil() const noexcept;

	unsigned int GetHoveredID() const noexcept;

	void Draw(const Sprite& aSprite);

protected:
	virtual void OnInit() override;
	virtual void OnRender() override;
	virtual void OnPostRender() override;

private:
	assets::ShaderRegistry* myShaderRegistry;

	CommonUtilities::Matrix4x4f myView;
	CommonUtilities::Matrix4x4f myProjection;

	graphics::PImpl<graphics::RenderTarget> myRenderTarget;
	graphics::PImpl<graphics::RenderTarget> myIDsTarget;
	graphics::PImpl<graphics::DepthStencil> myDepthStencil;

	graphics::PImpl<graphics::Texture2D> myIDStagingTexture;

	graphics::PImpl<graphics::BlendState> myBlendState;

	graphics::PImpl<graphics::ConstantBuffer> myFrameBuffer;
	graphics::PImpl<graphics::SamplerState> mySampler;

	graphics::PImpl<graphics::VertexShader> myVertexShader;
	//graphics::PImpl<graphics::GeometryShader> myGeometryShader;
	graphics::PImpl<graphics::PixelShader> myPixelShader;

	std::vector<Sprite::Instance> myInstances;

	unsigned int myHoveredID;
};