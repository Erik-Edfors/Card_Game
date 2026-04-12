#pragma once
#include "Component.h"
#include <Core/GraphicsEngine/RenderLayers/SpriteRenderLayer.h>
#include "../Reference.hpp"

#include <memory>

class TextRenderLayer;
class TransformComponent;

class Font;
class Text;

class TextComponent : public Component
{
public:
	TextComponent();
	~TextComponent();

	void SetFont(const wchar_t* aFontFile);
	void SetText(const wchar_t* aText) noexcept;

protected:
	void OnInit() override;
	void OnStart() override;
	void OnUpdate() override;
private:
	std::unique_ptr<Font> myFont;
	std::unique_ptr<Text> myText;

	TextRenderLayer* myRenderLayer;
	ComponentRef<TransformComponent> myTransform;
};