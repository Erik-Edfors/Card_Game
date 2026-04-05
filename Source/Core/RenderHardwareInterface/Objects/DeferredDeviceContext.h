#pragma once
#include "DeviceContext.h"

namespace graphics
{
	class DeferredDeviceContext : public virtual DeviceContext
	{
	public:
		virtual ~DeferredDeviceContext();
	protected:
		DeferredDeviceContext(GraphicsID aID);
	private:
	};
}