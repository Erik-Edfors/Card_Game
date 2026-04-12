#include "Engine.pch.h"
#include "TextComponent.h"

#include "Engine.h"
#include "Scene.h"
#include "ComponentSystem/ComponentSystem.h"
#include "TransformComponent.h"

#include <Core/GraphicsEngine/GraphicsEngine.h>
#include <Core/GraphicsEngine/RenderLayers/TextRenderLayer.h>

#include <fstream>

TextComponent::TextComponent() : 
	myRenderLayer(nullptr)
{
}

TextComponent::~TextComponent()
{
}

void TextComponent::SetFont(const wchar_t* aFontFile)
{
	myFont = std::make_unique<Font>(myRenderLayer->CreateFont(aFontFile));
}

void TextComponent::SetText(const wchar_t* aText) noexcept
{
	if (myFont == nullptr)
	{
		return;
	}

	myText = std::make_unique<Text>(
		myRenderLayer->CreateText(
			aText,
			*myFont,
			{ 1.f, 1.f, 1.f, 1.f },
			1.f,
			0.f
		)
	);
}

void TextComponent::OnInit()
{
	myRenderLayer = GetSystem().GetScene().GetEngine().GetGraphicsEngine().GetRenderLayer<TextRenderLayer>();
}

void TextComponent::OnStart()
{
	myTransform = GetActor()->GetComponent<TransformComponent>();
}

void TextComponent::OnUpdate()
{
	if (myText != nullptr && myTransform != nullptr)
	{
		myRenderLayer->Draw(*myText, myTransform->GetMatrixGlobal());
	}
}
