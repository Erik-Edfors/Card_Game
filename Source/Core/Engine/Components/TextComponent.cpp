#include "Engine.pch.h"
#include "TextComponent.h"
#include "TransformComponent.h"
#include <Core/ComponentSystem/ComponentSystem.h>
#include <Core/Engine/Engine.h>
#include <Core/GraphicsEngine/GraphicsEngine.h>
#include <Core/GraphicsEngine/RenderLayers/TextRenderLayer.h>

TextComponent::TextComponent(TextRenderLayer* aLayer) noexcept : 
	myRenderLayer(aLayer)
{
}

TextComponent::~TextComponent() noexcept
{
}

void TextComponent::SetColor(CommonUtilities::Vector4f aColor) noexcept
{
	myText.SetColor(aColor);
}
CommonUtilities::Vector4f TextComponent::GetColor() const noexcept
{
	return myText.GetColor();
}

void TextComponent::SetBorderColor(CommonUtilities::Vector4f aColor) noexcept
{
	myText.SetBorderColor(aColor);
}
CommonUtilities::Vector4f TextComponent::GetBorderColor() const noexcept
{
	return myText.GetBorderColor();
}

void TextComponent::SetBorderThickness(float aBorderThickness) noexcept
{
	myText.SetBorderThickness(aBorderThickness);
}
float TextComponent::GetBorderThickness() const noexcept
{
	return myText.GetBorderThickness();
}

void TextComponent::SetAdvancePadding(float aPadding) noexcept
{
	myText.SetAdvancePadding(aPadding);
}
float TextComponent::GetAdvancePadding() const noexcept
{
	return myText.GetAdvancePadding();
}

void TextComponent::SetAlignment(float aAlignment) noexcept
{
	myText.SetAlignment(aAlignment);
}
float TextComponent::GetAlignment() const noexcept
{
	return myText.GetAlignment();
}

void TextComponent::SetFont(const assets::FontFile* aFont) noexcept
{
	myText.SetFont(aFont);
}
const assets::FontFile* TextComponent::GetFont() const noexcept
{
	return myText.GetFont();
}

void TextComponent::SetFontSize(float aFontSize) noexcept
{
	myText.SetFontSize(aFontSize);
}
float TextComponent::GetFontSize() const noexcept
{
	return myText.GetFontSize();
}

void TextComponent::SetMaxWidth(float aMaxWidth) noexcept
{
	myText.SetMaxWidth(aMaxWidth);
}
float TextComponent::GetMaxWidth() const noexcept
{
	return myText.GetMaxWidth();
}

void TextComponent::SetText(const wchar_t* aText) noexcept
{
	myText.SetText(aText);
}
void TextComponent::SetText(const char* aText) noexcept
{
	myText.SetText(aText);
}
const std::wstring_view TextComponent::GetText() noexcept
{
	return myText.GetText();
}

void TextComponent::OnStart()
{
	myTransform = GetSystem().GetComponent<TransformComponent>(GetEntityHandle());
}

void TextComponent::OnUpdate(float aDeltaTime)
{
	aDeltaTime;
	if (myTransform != nullptr && myRenderLayer != nullptr)
	{
		myText.SetTransform(myTransform->GetGlobalMatrix());

		myRenderLayer->Draw(myText);
		//LOG_L(Verbose, "TextComponent", "Updated! [%i]", GetHandle().Handle);
	}
}
