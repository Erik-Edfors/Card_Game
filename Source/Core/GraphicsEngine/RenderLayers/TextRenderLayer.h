#pragma once
#include "RenderLayer.h"
#include <vector>
#include <Core/CommonUtilities/Math/Matrix4x4.hpp>
#include <Core/RenderHardwareInterface/Objects/ShaderResource.h>
#include <Core/RenderHardwareInterface/Objects/Buffers/VertexBuffer.h>
#include <Core/RenderHardwareInterface/Objects/Buffers/ConstantBuffer.h>
#include <Core/Assets/FileFormats/Font.h>	// TODO: Remove, this is temporary until a proper storage for fonts is made.

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

class TextRenderLayer;

class Text
{
	friend class TextRenderLayer;
public:
	struct GlyphVertex
	{
		float Bounds[4];
		float UVRect[4];
	};

	struct TextInstance
	{
		unsigned int VertexCount;
		graphics::PImpl<graphics::ShaderResource> Atlas;
		graphics::PImpl<graphics::VertexBuffer> VertexBuffer;
		graphics::PImpl<graphics::ConstantBuffer> ObjectBuffer;
	};

	Text() noexcept;
	~Text() noexcept;

	void SetTransform(const CommonUtilities::Matrix4x4f& aTransform) noexcept;
	const CommonUtilities::Matrix4x4f& GetTransform() const noexcept;

	void SetAlignment(float aAlignment) noexcept;
	float GetAlignment() const noexcept;

	void SetColor(CommonUtilities::Vector4f aColor) noexcept;
	CommonUtilities::Vector4f GetColor() const noexcept;

	void SetBorderColor(CommonUtilities::Vector4f aColor) noexcept;
	CommonUtilities::Vector4f GetBorderColor() const noexcept;

	void SetBorderThickness(float aBorderThickness) noexcept;
	float GetBorderThickness() const noexcept;

	void SetAdvancePadding(float aPadding) noexcept;
	float GetAdvancePadding() const noexcept;

	void SetFont(const assets::FontFile* aFont) noexcept;
	const assets::FontFile* GetFont() const noexcept;

	void SetFontSize(float aFontSize) noexcept;
	float GetFontSize() const noexcept;

	void SetMaxWidth(float aMaxWidth) noexcept;
	float GetMaxWidth() const noexcept;

	void SetText(const wchar_t* aText) noexcept;
	void SetText(const char* aText) noexcept;
	std::wstring_view GetText() const noexcept;

	TextInstance GetInstance(graphics::Device* aDevice) const;

private:
	void Clean(graphics::Device* aDevice) const;

	static void AlignVertices(float aLeft, float aRight, float aAlignment, GlyphVertex* aVertices, size_t aNumVertices) noexcept;

	mutable bool myObjectBufferIsDirty;
	mutable bool myVerticesIsDirty;
	mutable bool myAtlasIsDirty;
	// myInstance should only be updated to reflect the actual state of the Text object when used and needed as it is an intensive process.
	mutable TextInstance myInstance;

	CommonUtilities::Matrix4x4f myTransform;
	const assets::FontFile* myFont;
	std::wstring myText;
	CommonUtilities::Vector4f myColor;
	CommonUtilities::Vector4f myBorderColor;
	float myAlignment;
	float myBorderThickness;
	float myAdvancePadding;
	float myFontSize;
	float myMaxWidth;
};

class TextRenderLayer : public RenderLayer
{
public:
	TextRenderLayer(assets::ShaderRegistry* aShaderRegistry);
	~TextRenderLayer() = default;

	void SetRenderTarget(graphics::RenderTarget* aRenderTarget);
	void SetDepthStencil(graphics::DepthStencil* aDepthStencil);

	void SetView(const CommonUtilities::Matrix4x4<float>& aView);
	void SetProjection(const CommonUtilities::Matrix4x4<float>& aProjection);

	void Draw(const Text& aText);

	//Text CreateText(const char* aText, const assets::FontFile& aFont, float aSize, const CommonUtilities::Matrix4x4f& aTransform) const;

protected:
	virtual void OnInit() override;
	virtual void OnRender() override;
	//virtual void OnPreRender() override;
	virtual void OnPostRender() override;

private:
	assets::ShaderRegistry* myShaderRegistry;

	CommonUtilities::Matrix4x4<float> myView;
	CommonUtilities::Matrix4x4<float> myProjection;

	graphics::PImpl<graphics::RenderTarget> myRenderTarget;
	graphics::PImpl<graphics::DepthStencil> myDepthStencil;

	graphics::PImpl<graphics::BlendState> myBlendState;
	graphics::PImpl<graphics::InputLayout> myInputLayout;

	graphics::PImpl<graphics::VertexShader> myVertexShader;
	graphics::PImpl<graphics::GeometryShader> myGeometryShader;
	graphics::PImpl<graphics::PixelShader> myPixelShader;

	graphics::PImpl<graphics::ConstantBuffer> myFrameBuffer;
	graphics::PImpl<graphics::SamplerState> mySampler;

	std::vector<Text::TextInstance> myInstances;
};