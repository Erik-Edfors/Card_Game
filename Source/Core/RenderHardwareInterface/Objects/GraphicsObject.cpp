#include "RenderHardwareInterface.pch.h"
#include "GraphicsObject.h"

graphics::GraphicsObject::GraphicsObject(GraphicsID aID) : 
    myID(aID)
{
}

const std::string& graphics::GraphicsObject::GetName() const
{
    return myName;
}

graphics::GraphicsID graphics::GraphicsObject::GetID() const
{
    return myID;
}

void graphics::GraphicsObject::SetLocalName(const char* aName)
{
    myName = aName;
}
