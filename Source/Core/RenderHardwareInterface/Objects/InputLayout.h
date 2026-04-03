#pragma once
#include "GraphicsObject.h"
#include <vector>
#include <string>
#include <initializer_list>

namespace graphics
{
	struct LayoutElementDesc
	{
		std::string SemanticName;
		unsigned int SemanticIndex =			0u;
		GRAPHICS_FORMAT Format =				GRAPHICS_FORMAT_UNKNOWN;
		unsigned int InputSlot =				0u;
		unsigned int AlignedByteOffset =		GRAPHICS_INPUT_LAYOUT_APPEND_ALIGNED_ELEMENT;
		GRAPHICS_INPUT_CLASS InputSlotClass =	GRAPHICS_INPUT_PER_VERTEX;
		unsigned int InstanceDataStepRate =		0u;
	};

	struct LayoutDefinition
	{
		LayoutDefinition() = default;
		LayoutDefinition(const std::initializer_list<LayoutElementDesc>& aInitializerList);

		std::vector<LayoutElementDesc> Elements;
	};

	struct InputLayoutDesc
	{
		std::string Name;
		LayoutDefinition LayoutDefinition;
		void* ShaderByteCode;
		size_t NumBytes;
	};

	class InputLayout : public GraphicsObject
	{
	public:
		const LayoutDefinition& GetLayoutDefinition() const;
	protected:
		InputLayout(GraphicsID aID, const LayoutDefinition& aLayoutDefinition);
	private:
		LayoutDefinition myLayoutDefinition;
	};
}