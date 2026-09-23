#pragma once
#include <Core/ComponentSystem/Component.h>
#include <Core/ComponentSystem/ComponentRef.hpp>
#include <Core/GraphicsEngine/RenderLayers/TextRenderLayer.h>
#include <Core/CommonUtilities/Math/Vector4.hpp>

class TransformComponent;

class TextComponent : public cs::Component
{
public:
	TextComponent(TextRenderLayer* aLayer) noexcept;
	~TextComponent() noexcept;

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
	const std::wstring_view GetText() noexcept;

protected:
	virtual void OnStart() override;
	virtual void OnUpdate(float aDeltaTime) override;

private:
	cs::ComponentRef<TransformComponent> myTransform;
	TextRenderLayer* myRenderLayer;
	Text myText;
};