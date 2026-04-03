#include "RenderHardwareInterface.pch.h"
#include "Device.h"
#include "Objects/GraphicsObject.h"

graphics::DeviceID graphics::Device::locDeviceIDCounter = 0;

graphics::Device::Device() : 
    myDeviceID(locDeviceIDCounter++),
    myObjectIDCounter(0)
{
}

graphics::DeviceID graphics::Device::GetID() const noexcept
{
    return myDeviceID;
}

bool graphics::Device::IsDeviceObject(GraphicsObject* aGraphicsObject) const noexcept
{
    return aGraphicsObject != nullptr && GetID() == aGraphicsObject->GetID().Device;
}

graphics::GraphicsID graphics::Device::CreateGraphicsID() noexcept
{
    return _createGraphicsID();
}

graphics::DeviceID graphics::Device::_createDeviceID()
{
    return locDeviceIDCounter++;
}

graphics::ObjectID graphics::Device::_createObjectID()
{
    return myObjectIDCounter++;
}

graphics::GraphicsID graphics::Device::_createGraphicsID()
{
    GraphicsID id = {};
    id.Device = myDeviceID;
    id.Object = _createObjectID();
    return id;
}
