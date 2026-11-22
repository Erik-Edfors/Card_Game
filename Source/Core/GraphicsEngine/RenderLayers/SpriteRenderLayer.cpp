#include "GraphicsEngine.pch.h"
#include "SpriteRenderLayer.h"

#include "GraphicsEngine.h"

#include <Core/Assets/ShaderRegistry.h>

#include <Core/RenderHardwareInterface/Device.h>
#include <Core/RenderHardwareInterface/Window.h>
#include <Core/RenderHardwareInterface/Objects/ImmediateDeviceContext.h>
#include <Core/RenderHardwareInterface/Objects/DeferredDeviceContext.h>
#include <Core/RenderHardwareInterface/Objects/Texture2D.h>
#include <Core/RenderHardwareInterface/Objects/RenderTarget.h>
#include <Core/RenderHardwareInterface/Objects/DepthStencil.h>
#include <Core/RenderHardwareInterface/Objects/InputLayout.h>
#include <Core/RenderHardwareInterface/Objects/BlendState.h>
#include <Core/RenderHardwareInterface/Objects/SamplerState.h>
#include <Core/RenderHardwareInterface/Objects/Shaders/VertexShader.h>
#include <Core/RenderHardwareInterface/Objects/Shaders/PixelShader.h>

static unsigned int locNextID = 1;

struct FrameBuffer
{
	CommonUtilities::Matrix4x4f View;
	CommonUtilities::Matrix4x4f Projection;
	CommonUtilities::Vector2f TargetSize;

	unsigned int _padding[2];
};

struct ObjectBuffer
{
	CommonUtilities::Matrix4x4f Transform;
	CommonUtilities::Vector4f Tint;
	CommonUtilities::AABB2Df AtlasRect;
	CommonUtilities::Vector2f Origin;
	unsigned int ID;

	unsigned int _padding[1];
};

Sprite::Sprite() :
	myObjectBufferIsDirty(true),
	myAtlasRect({ 0.f, 0.f }, { 1.f, 1.f }),
	myTint(1.f, 1.f, 1.f, 1.f),
	myID(locNextID++)
{
}
Sprite::~Sprite()
{
}

void Sprite::SetAtlas(graphics::ShaderResource* aAtlas) noexcept
{
	myInstance.Atlas = aAtlas;
}
graphics::ShaderResource* Sprite::GetAtlas() const noexcept
{
	return myInstance.Atlas.Get();
}

void Sprite::SetAtlasRect(CommonUtilities::AABB2Df aRect) noexcept
{
	if (myObjectBufferIsDirty || myAtlasRect != aRect)
	{
		myObjectBufferIsDirty = true;
		myAtlasRect = aRect;
	}
}
CommonUtilities::AABB2Df Sprite::GetAtlasRect() const noexcept
{
	return myAtlasRect;
}

void Sprite::SetTint(CommonUtilities::Vector4f aTint) noexcept
{
	if (myObjectBufferIsDirty || myTint != aTint)
	{
		myObjectBufferIsDirty = true;
		myTint = aTint;
	}
}
CommonUtilities::Vector4f Sprite::GetTint() const noexcept
{
	return myTint;
}

void Sprite::SetOrigin(CommonUtilities::Vector2f aOrigin) noexcept
{
	if (myObjectBufferIsDirty || myOrigin != aOrigin)
	{
		myObjectBufferIsDirty = true;
		myOrigin = aOrigin;
	}
}
CommonUtilities::Vector2f Sprite::GetOrigin() const noexcept
{
	return myOrigin;
}

void Sprite::SetTransform(const CommonUtilities::Matrix4x4f& aTransform) noexcept
{
	if (myTransform != aTransform)
	{
		myObjectBufferIsDirty = true;
		myTransform = aTransform;
	}
}
const CommonUtilities::Matrix4x4f& Sprite::GetTransform() const noexcept
{
	return myTransform;
}

unsigned int Sprite::GetID() const noexcept
{
	return myID;
}

Sprite::Instance Sprite::GetInstance(graphics::Device* aDevice) const
{
	Clean(aDevice);
	return myInstance;
}

void Sprite::Clean(graphics::Device* aDevice) const
{
	if (myObjectBufferIsDirty)
	{
		ObjectBuffer buffer = {
			.Transform = myTransform,
			.Tint = myTint,
			.AtlasRect = myAtlasRect,
			.Origin = myOrigin,
			.ID = myID
		};

		if (not myInstance.ObjectBuffer)
		{
			const graphics::ConstantBufferDesc desc = {
				.Name = "Sprite ObjectBuffer",
				.InitialData = &buffer,
				.Size = sizeof(buffer),
				.Usage = graphics::GRAPHICS_USAGE_DYNAMIC,
				.CPUAccessFlags = graphics::GRAPHICS_CPU_ACCESS_WRITE
			};
			graphics::result::Code result = aDevice->CreateConstantBuffer(&myInstance.ObjectBuffer, &desc);
			result;
		}
		else
		{
			graphics::result::Code result = aDevice->UpdateConstantBuffer(myInstance.ObjectBuffer.Get(), &buffer, sizeof(buffer));
			result;
		}

		myObjectBufferIsDirty = false;
	}
}



SpriteRenderLayer::SpriteRenderLayer(assets::ShaderRegistry* aShaderRegistry) :
	myShaderRegistry(aShaderRegistry),
	myHoveredID(0u)
{
}
SpriteRenderLayer::~SpriteRenderLayer()
{
}

void SpriteRenderLayer::SetView(const CommonUtilities::Matrix4x4f& aView) noexcept
{
	myView = aView;
}
const CommonUtilities::Matrix4x4f& SpriteRenderLayer::GetView() const noexcept
{
	return myView;
}

void SpriteRenderLayer::SetProjection(const CommonUtilities::Matrix4x4f& aProjection)
{
	myProjection = aProjection;
}
const CommonUtilities::Matrix4x4f& SpriteRenderLayer::GetProjection() const noexcept
{
	return myProjection;
}

void SpriteRenderLayer::SetRenderTarget(graphics::RenderTarget* aRenderTarget) noexcept
{
	myRenderTarget = aRenderTarget;
}
graphics::RenderTarget* SpriteRenderLayer::GetRenderTarget() const noexcept
{
	return myRenderTarget.Get();
}

void SpriteRenderLayer::SetDepthStencil(graphics::DepthStencil* aDepthStencil) noexcept
{
	myDepthStencil = aDepthStencil;
}
graphics::DepthStencil* SpriteRenderLayer::GetDepthStencil() const noexcept
{
	return myDepthStencil.Get();
}

unsigned int SpriteRenderLayer::GetHoveredID() const noexcept
{
	return myHoveredID;
}

void SpriteRenderLayer::Draw(const Sprite& aSprite)
{
	myInstances.push_back(aSprite.GetInstance(GetDevice()));
}

void SpriteRenderLayer::OnInit()
{
	const assets::ShaderAsset* shader = myShaderRegistry->Get("core:sprite_vs");
	graphics::result::Code result = GetDevice()->CreateShader(&myVertexShader, "Sprite VertexShader", shader->ByteCode.get(), shader->ByteCodeSize);

	shader = myShaderRegistry->Get("core:sprite_ps");
	result = GetDevice()->CreateShader(&myPixelShader, "Sprite PixelShader", shader->ByteCode.get(), shader->ByteCodeSize);

	{
		const graphics::Texture2DDesc desc = {
			.BindFlags = graphics::GRAPHICS_BIND_NONE,
			.ResourceFlags = graphics::GRAPHICS_RESOURCE_NONE,
			.Name = "1x1 ID Staging Texture",
			.Width = 1,
			.Height = 1,
			.MipLevels = 1,
			.NumTextures = 1,
			.Data = nullptr,
			.NumData = 0,
			.Format = graphics::GRAPHICS_FORMAT_R32_UINT,
			.Usage = graphics::GRAPHICS_USAGE_STAGING,
			.CPUAccessFlags = graphics::GRAPHICS_CPU_ACCESS_READ
		};
		result = GetDevice()->CreateTexture2D(&myIDStagingTexture, &desc);
		if (GRAPHICS_FAILED(result))
		{
			//TODO: LOG
		}
	}

	{
		graphics::BlendStateDesc desc = {
			.Name = "Sprite BlendState",
			.EnableAlphaToCoverage = false,
			.EnableIndependentBlend = true,
			.RenderTargetBlendDescriptions = {
				{
					.EnableBlend = true,
					.SourceBlend = graphics::GRAPHICS_BLEND_SRC_ALPHA,
					.DestinationBlend = graphics::GRAPHICS_BLEND_INV_SRC_ALPHA,
					.BlendOperation = graphics::GRAPHICS_BLEND_OPERATION_ADD,
					.AlphaSourceBlend = graphics::GRAPHICS_BLEND_ONE,
					.AlphaDestinationBlend = graphics::GRAPHICS_BLEND_ZERO,
					.AlphaBlendOperation = graphics::GRAPHICS_BLEND_OPERATION_ADD,
					.RenderTargetWriteMask = graphics::GRAPHICS_BLEND_COLOR_WRITE_ALL
				},
				{
					.EnableBlend = false,
					.SourceBlend = graphics::GRAPHICS_BLEND_ONE,
					.DestinationBlend = graphics::GRAPHICS_BLEND_ZERO,
					.BlendOperation = graphics::GRAPHICS_BLEND_OPERATION_ADD,
					.AlphaSourceBlend = graphics::GRAPHICS_BLEND_ONE,
					.AlphaDestinationBlend = graphics::GRAPHICS_BLEND_ZERO,
					.AlphaBlendOperation = graphics::GRAPHICS_BLEND_OPERATION_ADD,
					.RenderTargetWriteMask = graphics::GRAPHICS_BLEND_COLOR_WRITE_ALL
				}
			}
		};
		result = GetDevice()->CreateBlendState(&myBlendState, &desc);
		if (GRAPHICS_FAILED(result))
		{
			//TODO: LOG
		}
	}

	{
		graphics::ConstantBufferDesc desc = {
			.Name = "Frame Buffer",
			.InitialData = nullptr,
			.Size = sizeof(FrameBuffer),
			.Usage = graphics::GRAPHICS_USAGE_DYNAMIC,
			.CPUAccessFlags = graphics::GRAPHICS_CPU_ACCESS_WRITE
		};
		result = GetDevice()->CreateConstantBuffer(&myFrameBuffer, &desc);
		if (GRAPHICS_FAILED(result))
		{
			//TODO: LOG
		}
	}

	{
		graphics::SamplerStateDesc desc = {
			.Name = "Trilinear Sampler",
			.Filter = graphics::GRAPHICS_SAMPLER_FILTER_MIN_MAG_MIP_LINEAR,
			.AddressU = graphics::GRAPHICS_TEXTURE_ADDRESS_WRAP,
			.AddressV = graphics::GRAPHICS_TEXTURE_ADDRESS_WRAP,
			.AddressW = graphics::GRAPHICS_TEXTURE_ADDRESS_WRAP,
			.MipLODBias = 0.f,
			.MaxAnistropy = 0,
			.Comparison = graphics::GRAPHICS_COMPARISON_NEVER,
			.BorderColor = { 0.f, 0.f, 0.f, 0.f },
			.MinLOD = 0.f,
			.MaxLOD = FLT_MAX
		};

		result = GetDevice()->CreateSamplerState(&mySampler, &desc);
		if (GRAPHICS_FAILED(result))
		{
			//TODO: LOG
		}
	}
}

void SpriteRenderLayer::OnRender()
{
	if (not myRenderTarget)
	{
		return;
	}
	if (myInstances.empty())
	{
		return;
	}

	graphics::result::Code result;

	GetContext()->PushEvent("Sprite Rendering");

	const graphics::Texture2DDesc& rtDesc = myRenderTarget->GetDesc().Texture->GetDesc();

	bool needToCreateIDsTarget = myIDsTarget == nullptr;

	if (not needToCreateIDsTarget)
	{
		const graphics::Texture2DDesc& idsDesc = myIDsTarget->GetDesc().Texture->GetDesc();
		needToCreateIDsTarget |= idsDesc.Width != rtDesc.Width || idsDesc.Height != rtDesc.Height;
	}

	if (needToCreateIDsTarget)
	{
		const graphics::Texture2DDesc textureDesc = {
			.BindFlags = graphics::GRAPHICS_BIND_RENDER_TARGET,
			.ResourceFlags = graphics::GRAPHICS_RESOURCE_NONE,
			.Name = "Sprite IDs Texture",
			.Width = rtDesc.Width,
			.Height = rtDesc.Height,
			.MipLevels = 1,
			.NumTextures = 1,
			.Data = nullptr,
			.NumData = 0,
			.Format = graphics::GRAPHICS_FORMAT_R32_UINT,
			.Usage = graphics::GRAPHICS_USAGE_DEFAULT,
			.CPUAccessFlags = graphics::GRAPHICS_CPU_ACCESS_NONE
		};
		graphics::PImpl<graphics::Texture2D> texture;
		result = GetDevice()->CreateTexture2D(&texture, &textureDesc);
		if (GRAPHICS_FAILED(result))
		{
			//TODO: LOG
		}

		const graphics::RenderTargetDesc renderTargetDesc = {
			.Name = "Sprite IDs RenderTarget",
			.Texture = texture.Get(),
			.Format = graphics::GRAPHICS_FORMAT_R32_UINT,
			.ViewDimension = graphics::GRAPHICS_RENDER_TARGET_VIEW_DIMENSION_TEXTURE2D,
			.FirstSlice = 0,
			.NumSlices = 1,
			.MipSlice = 0
		};
		result = GetDevice()->CreateRenderTarget(&myIDsTarget, &renderTargetDesc);
		if (GRAPHICS_FAILED(result))
		{
			//TODO: LOG
		}
	}

	const float clearColor[4] = { 0.f, 0.f, 0.f, 0.f };
	result = GetContext()->ClearRenderTarget(myIDsTarget.Get(), clearColor);

	result = GetContext()->SetTopology(graphics::GRAPHICS_TOPOLOGY_TRIANGLELIST);

	FrameBuffer frameBuffer = {
		.View = myView,
		.Projection = myProjection,
		.TargetSize = { 
			static_cast<float>(rtDesc.Width), 
			static_cast<float>(rtDesc.Height)
		},
		._padding = {}
	};
	result = GetContext()->UpdateConstantBuffer(myFrameBuffer.Get(), &frameBuffer, sizeof(frameBuffer));
	result = GetContext()->BindConstantBuffers(&myFrameBuffer, 1, 0, graphics::GRAPHICS_SHADER_STAGE_VERTEX);

	result = GetContext()->SetBlendState(myBlendState.Get());

	result = GetContext()->BindSamplerStates(&mySampler, 1, 0, graphics::GRAPHICS_SHADER_STAGE_PIXEL);

	result = GetContext()->BindVertexShader(myVertexShader.Get());
	//result = GetContext()->BindGeometryShader(myGeometryShader.Get());
	result = GetContext()->BindPixelShader(myPixelShader.Get());

	const graphics::Viewport viewport = {
		.Left = 0.f,
		.Right = static_cast<float>(rtDesc.Width),
		.Top = 0.f,
		.Bottom = static_cast<float>(rtDesc.Height),
		.Front = 0.f,
		.Back = 1.f
	};

	const graphics::Viewport viewports[2] = {
		viewport,
		viewport
	};

	result = GetContext()->SetViewports(viewports, 2);

	graphics::RenderTarget* const targets[2] = {
		myRenderTarget.Get(),
		myIDsTarget.Get()
	};
	result = GetContext()->BindTargets(targets, 2, myDepthStencil.Get());

	for (const Sprite::Instance& instance : myInstances)
	{
		result = GetContext()->BindShaderResources(&instance.Atlas, 1, 0, graphics::GRAPHICS_SHADER_STAGE_PIXEL);
		result = GetContext()->BindConstantBuffers(&instance.ObjectBuffer, 1, 1, graphics::GRAPHICS_SHADER_STAGE_VERTEX | graphics::GRAPHICS_SHADER_STAGE_PIXEL);

		GetContext()->Draw(6, 0);
	}

	GetContext()->PopEvent();
}

void SpriteRenderLayer::OnPostRender()
{
	myInstances.clear();
	myHoveredID = 0;

	graphics::Window& window = *GetGraphicsEngine()->GetWindow();
	const float mouseX = window.GetMouseX() / window.GetClientWidth();
	const float mouseY = window.GetMouseY() / window.GetClientHeight();

	graphics::PImpl<graphics::ImmediateDeviceContext> context;
	GetDevice()->CreateImmediateDeviceContext(&context);

	if (myIDsTarget && mouseX >= 0.f && mouseX <= 1.f && mouseY >= 0.f && mouseY <= 1.f)
	{
		const graphics::Texture2DDesc& desc = myIDsTarget->GetTexture()->GetDesc();

		const unsigned int x = static_cast<unsigned int>(mouseX * static_cast<float>(desc.Width));
		const unsigned int y = static_cast<unsigned int>(mouseY * static_cast<float>(desc.Height));

		if (x < desc.Width && y < desc.Height)
		{
			graphics::result::Code result = context->CopyTexture(
				myIDsTarget->GetTexture(),
				x, y,
				x + 1, y + 1,
				myIDStagingTexture.Get(),
				0, 0
			);
			if (GRAPHICS_FAILED(result))
			{
				//TODO: Log.
			}

			void* data;
			unsigned int width;
			unsigned int size;
			result = GetDevice()->Map(
				myIDStagingTexture.Get(),
				0,
				graphics::GRAPHICS_MAP_READ,
				graphics::GRAPHICS_MAP_FLAG_NONE,
				&data,
				&width,
				&size
			);

			if (GRAPHICS_SUCCEEDED(result))
			{
				//TODO: Log.
				if (size >= sizeof(unsigned int))
				{
					myHoveredID = *reinterpret_cast<unsigned int*>(data);
				}
			}

			result = GetDevice()->Unmap(myIDStagingTexture.Get(), 0);
			if (GRAPHICS_FAILED(result))
			{
				//TODO: Log.
			}
		}
	}
}
