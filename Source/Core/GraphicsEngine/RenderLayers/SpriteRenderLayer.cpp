#include "GraphicsEngine.pch.h"
#include "SpriteRenderLayer.h"

#include "GraphicsEngine.h"

#include "ConstantBuffers/FrameBuffer.h"
#include "ConstantBuffers/ObjectBuffer.h"
#include "ConstantBuffers/MaterialBuffer.h"

#include <Core/RenderHardwareInterface/Device.h>
#include <Core/RenderHardwareInterface/Objects/DeferredDeviceContext.h>
#include <Core/RenderHardwareInterface/Objects/Texture2D.h>
#include <Core/RenderHardwareInterface/Objects/RenderTarget.h>

#include <Core/RenderHardwareInterface/Objects/Buffers/ConstantBuffer.h>
#include <Core/RenderHardwareInterface/Objects/Buffers/VertexBuffer.h>

#include <algorithm>

struct SpriteVertex
{
	float Position[2];
	float Size[2];
	float UVRect[4];
	float Pivot[2];
	float Rotation;
};

SpriteRenderLayer::SpriteRenderLayer()
{
}

SpriteRenderLayer::~SpriteRenderLayer()
{
}

void SpriteRenderLayer::SetTarget(graphics::RenderTarget* aRenderTarget) noexcept
{
	myRenderTarget = aRenderTarget;
}

void SpriteRenderLayer::SetAtlas(graphics::ShaderResource* aAtlas) noexcept
{
	myTextureAtlas = aAtlas;
}

void SpriteRenderLayer::SetView(const CommonUtilities::Matrix4x4f& aView) noexcept
{
	myView = aView;
}

void SpriteRenderLayer::SetProjection(const CommonUtilities::Matrix4x4f& aProjection) noexcept
{
	myProjection = aProjection;
}

void SpriteRenderLayer::Draw(const Sprite& aSprite)
{
	mySprites.push_back(aSprite);
}

void SpriteRenderLayer::OnInit()
{
	constexpr size_t shaderBufferSize = 16'384;
	char* shaderBuffer = new char[shaderBufferSize];

	{
		//TODO: use asset system or something to get shader file.
		std::ifstream file("../../Assets/Shaders/Sprite_VS.cso");

		size_t shaderSize = 0;

		memset(shaderBuffer, 0, shaderBufferSize);
		while (file.read(shaderBuffer, sizeof(shaderBuffer)))
		{
			shaderSize += file.gcount();
		}

		GetGraphicsEngine()->GetDevice()->CreateShader(
			&myVertexShader,
			"Sprite VertexShader",
			shaderBuffer,
			shaderSize
		);

		const graphics::InputLayoutDesc desc = {
			.Name = "Sprite InputLayout",
			.LayoutDefinition = {
				graphics::LayoutElementDesc{
					.SemanticName = "Position",
					.Format = graphics::GRAPHICS_FORMAT_R32G32_FLOAT
				},
				graphics::LayoutElementDesc{
					.SemanticName = "Size",
					.Format = graphics::GRAPHICS_FORMAT_R32G32_FLOAT
				},
				graphics::LayoutElementDesc{
					.SemanticName = "UVRect",
					.Format = graphics::GRAPHICS_FORMAT_R32G32B32A32_FLOAT
				},
				graphics::LayoutElementDesc{
					.SemanticName = "Pivot",
					.Format = graphics::GRAPHICS_FORMAT_R32G32_FLOAT
				},
				graphics::LayoutElementDesc{
					.SemanticName = "Rotation",
					.Format = graphics::GRAPHICS_FORMAT_R32_FLOAT
				}
			},
			.ShaderByteCode = shaderBuffer,
			.NumBytes = shaderSize
		};
		GetGraphicsEngine()->GetDevice()->CreateInputLayout(&myInputLayout, &desc);
	}
	{
		//TODO: use asset system or something to get shader file.
		std::ifstream file("../../Assets/Shaders/Sprite_GS.cso");

		size_t shaderSize = 0;

		memset(shaderBuffer, 0, shaderBufferSize);
		while (file.read(shaderBuffer, sizeof(shaderBuffer)))
		{
			shaderSize += file.gcount();
		}

		GetGraphicsEngine()->GetDevice()->CreateShader(
			&myGeometryShader,
			"Sprite GeometryShader",
			shaderBuffer,
			shaderSize
		);
	}
	{
		std::ifstream file("../../Assets/Shaders/Sprite_PS.cso");

		size_t shaderSize = 0;

		memset(shaderBuffer, 0, shaderBufferSize);
		while (file.read(shaderBuffer, sizeof(shaderBuffer)))
		{
			shaderSize += file.gcount();
		}

		GetGraphicsEngine()->GetDevice()->CreateShader(
			&myPixelShader,
			"Sprite PixelShader",
			shaderBuffer,
			shaderSize
		);
	}

	{
		const graphics::VertexBufferDesc desc = {
			.Name = "Sprite VertexBuffer",
			.Data = nullptr,
			.Size = sizeof(SpriteVertex) * 4096,
			.Usage = graphics::GRAPHICS_USAGE_DYNAMIC,
			.CPUAccessFlags = graphics::GRAPHICS_CPU_ACCESS_WRITE
		};
		GetGraphicsEngine()->GetDevice()->CreateVertexBuffer(&myVertexBuffer, &desc);
	}

	{
		const graphics::ConstantBufferDesc desc = {
			.Name = "Sprite FrameBuffer",
			.InitialData = nullptr,
			.Size = sizeof(FrameBuffer),
			.Usage = graphics::GRAPHICS_USAGE_DYNAMIC,
			.CPUAccessFlags = graphics::GRAPHICS_CPU_ACCESS_WRITE
		};
		GetGraphicsEngine()->GetDevice()->CreateConstantBuffer(&myFrameBuffer, &desc);
	}
	//{
	//	const graphics::ConstantBufferDesc desc = {
	//		.Name = "Sprite Object Buffer",
	//		.InitialData = nullptr,
	//		.Size = sizeof(ObjectBuffer),
	//		.Usage = graphics::GRAPHICS_USAGE_DYNAMIC,
	//		.CPUAccessFlags = graphics::GRAPHICS_CPU_ACCESS_WRITE
	//	};
	//	GetGraphicsEngine()->GetDevice()->CreateConstantBuffer(&myObjectBuffer, &desc);
	//}
	{
		const graphics::ConstantBufferDesc desc = {
			.Name = "Sprite Material Buffer",
			.InitialData = nullptr,
			.Size = sizeof(MaterialBuffer),
			.Usage = graphics::GRAPHICS_USAGE_DYNAMIC,
			.CPUAccessFlags = graphics::GRAPHICS_CPU_ACCESS_WRITE
		};
		GetGraphicsEngine()->GetDevice()->CreateConstantBuffer(&myMaterialBuffer, &desc);
	}

	{
		const graphics::BlendStateDesc desc = {
			.Name = "Sprite Blend State",
			.EnableAlphaToCoverage = false,
			.EnableIndependentBlend = false,
			.RenderTargetBlendDescriptions = {
				graphics::RenderTargetBlendDesc{
					.EnableBlend = true,
					.SourceBlend = graphics::GRAPHICS_BLEND_SRC_ALPHA,
					.DestinationBlend = graphics::GRAPHICS_BLEND_DEST_ALPHA,
					.BlendOperation = graphics::GRAPHICS_BLEND_OPERATION_ADD,
					.AlphaSourceBlend = graphics::GRAPHICS_BLEND_INV_DEST_ALPHA,
					.AlphaDestinationBlend = graphics::GRAPHICS_BLEND_DEST_ALPHA,
					.AlphaBlendOperation = graphics::GRAPHICS_BLEND_OPERATION_ADD
				}
			}
		};
		GetGraphicsEngine()->GetDevice()->CreateBlendState(&myBlendState, &desc);
	}

	graphics::PImpl<graphics::RasterizerState> rasterizerState = nullptr;
	graphics::PImpl<graphics::DepthStencilState> depthStencilState = nullptr;
	graphics::PImpl<graphics::BlendState> blendState = nullptr;

	graphics::PImpl<graphics::InputLayout> inputLayout = nullptr;

	std::vector<PipelineStateConstantBuffer> constantBuffers = {};

	graphics::PImpl<graphics::VertexShader> vertexShader;
	graphics::PImpl<graphics::HullShader> hullShader;
	graphics::PImpl<graphics::DomainShader> domainShader;
	graphics::PImpl<graphics::GeometryShader> geometryShader;
	graphics::PImpl<graphics::PixelShader> pixelShader;
	graphics::PImpl<graphics::ComputeShader> computeShader;

	std::vector<PipelineStateShaderResource> shaderResources = {};
	std::vector<PipelineStateSamplerState> samplerStates = {};
	std::vector<PipelineStateRenderTarget> renderTargets = {};
	std::vector<graphics::Viewport> viewports = {};
	PipelineStateDepthStencil depthStencil = {};

	myPipelineState = PipelineStateDesc{
		.Name = "Sprite Pipeline State",
		.Topology = graphics::GRAPHICS_TOPOLOGY_TRIANGLELIST,
		.RasterizerState = rasterizerState,
		.DepthStencilState = depthStencilState,
		.BlendState = blendState,

		.InputLayout = inputLayout,

		.ConstantBuffers = std::move(constantBuffers),

		.VertexShader = vertexShader,
		.HullShader = hullShader,
		.DomainShader = domainShader,
		.GeometryShader = geometryShader,
		.PixelShader = pixelShader,
		.ComputeShader = computeShader,

		.ShaderResources = std::move(shaderResources),
		.SamplerStates = std::move(samplerStates),
		.RenderTargets = std::move(renderTargets),
		.Viewports = std::move(viewports),
		.DepthStencil = depthStencil
	};
}

void SpriteRenderLayer::OnRender()
{
	GetContext()->ResetState();

	GetContext()->PushEvent("Sprite Rendering");

	graphics::ConstantBuffer* buffers[] = {
		myFrameBuffer.Get(),
		nullptr,
		myMaterialBuffer.Get()
	};
	GetContext()->BindConstantBuffers(buffers, 3, 0, graphics::GRAPHICS_SHADER_STAGE_VERTEX | graphics::GRAPHICS_SHADER_STAGE_PIXEL);

	const CommonUtilities::Vector4f cameraPosition = myView.GetInverse().Row(4);

	FrameBuffer frameBuffer = {
		.View = {
			{ myView(1, 1), myView(1, 2), myView(1, 3), myView(1, 4) },
			{ myView(2, 1), myView(2, 2), myView(2, 3), myView(2, 4) },
			{ myView(3, 1), myView(3, 2), myView(3, 3), myView(3, 4) },
			{ myView(4, 1), myView(4, 2), myView(4, 3), myView(4, 4) }
		},
		.Projection = {
			{ myProjection(1, 1), myProjection(1, 2), myProjection(1, 3), myProjection(1, 4) },
			{ myProjection(2, 1), myProjection(2, 2), myProjection(2, 3), myProjection(2, 4) },
			{ myProjection(3, 1), myProjection(3, 2), myProjection(3, 3), myProjection(3, 4) },
			{ myProjection(4, 1), myProjection(4, 2), myProjection(4, 3), myProjection(4, 4) }
		},
		.CameraPosition = { cameraPosition.x, cameraPosition.y, cameraPosition.z, cameraPosition.w }
	};

	GetContext()->UpdateConstantBuffer(myFrameBuffer.Get(), &frameBuffer, sizeof(frameBuffer));

	GetContext()->SetBlendState(myBlendState.Get());

	GetContext()->BindVertexShader(myVertexShader.Get());
	GetContext()->BindGeometryShader(myGeometryShader.Get());
	GetContext()->BindPixelShader(myPixelShader.Get());

	GetContext()->BindTargets(&myRenderTarget, 1, nullptr);
	GetContext()->BindShaderResources(&myTextureAtlas, 1, 0, graphics::GRAPHICS_SHADER_STAGE_PIXEL);

	graphics::Viewport viewport = {
		.Left = 0.f,
		.Right = static_cast<float>(myRenderTarget->GetTexture()->GetDesc().Width),
		.Top = 0.f,
		.Bottom = static_cast<float>(myRenderTarget->GetTexture()->GetDesc().Height),
		.Front = 0.f,
		.Back = 1.f
	};
	GetContext()->SetViewports(&viewport, 1);

	std::vector<Sprite> sprites = mySprites;

	std::sort(
		sprites.begin(),
		sprites.end(),
		[](const Sprite& s0, const Sprite& s1) {
			return s0.Depth < s1.Depth;
		}
	);

	std::vector<SpriteVertex> vertices;
	vertices.resize(sprites.size());

	for (size_t i = 0; i < sprites.size(); i++)
	{
		const Sprite& sprite = sprites[i];
		SpriteVertex& vertex = vertices[i];

		vertex.Position[0] = sprite.Position.x;
		vertex.Position[1] = sprite.Position.y;

		vertex.Size[0] = sprite.Size.x;
		vertex.Size[1] = sprite.Size.y;

		vertex.UVRect[0] = sprite.TextureRect.Min.x;
		vertex.UVRect[1] = sprite.TextureRect.Min.y;
		vertex.UVRect[2] = sprite.TextureRect.Max.x;
		vertex.UVRect[3] = sprite.TextureRect.Max.y;

		vertex.Pivot[0] = sprite.Pivot.x;
		vertex.Pivot[1] = sprite.Pivot.y;

		vertex.Rotation = sprite.Rotation;
	}

	GetContext()->UpdateVertexBuffer(myVertexBuffer.Get(), vertices.data(), vertices.size() * sizeof(vertices[0]));

	GetContext()->Draw(static_cast<unsigned int>(vertices.size()), 0u);

	GetContext()->PopEvent();
}
