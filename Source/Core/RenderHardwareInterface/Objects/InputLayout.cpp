#include "RenderHardwareInterface.pch.h"
#include "InputLayout.h"

const graphics::LayoutDefinition& graphics::InputLayout::GetLayoutDefinition() const
{
    return myLayoutDefinition;
}

graphics::InputLayout::InputLayout(GraphicsID aID, const LayoutDefinition& aLayoutDefinition) :
    myLayoutDefinition(aLayoutDefinition),
    GraphicsObject(aID)
{
}

graphics::LayoutDefinition::LayoutDefinition(const std::initializer_list<LayoutElementDesc>& aInitializerList) :
    Elements(aInitializerList)
{
}
