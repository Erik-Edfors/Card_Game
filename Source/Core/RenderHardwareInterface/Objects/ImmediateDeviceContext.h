#pragma once
#include "DeviceContext.h"

namespace graphics
{
	class DeferredDeviceContext;

	class ImmediateDeviceContext : public virtual DeviceContext
	{
	public:
		virtual ~ImmediateDeviceContext();

		//########
		// Resource Management
		//########

		virtual result::Code Map(
			Texture2D* aTexture,
			unsigned int aSubResource,
			GRAPHICS_MAP aMapType,
			GRAPHICS_MAP_FLAGS aMapFlags,
			void** outData,
			unsigned int* outRowPitch,
			unsigned int* outDepthPitch
		) = 0;

		virtual result::Code Unmap(
			Texture2D* aTexture,
			unsigned int aSubResource
		) = 0;

		virtual result::Code ExecuteDeferredDeviceContext(
			DeferredDeviceContext* aDeferredDeviceContext,
			bool aRestoreImmediateContextAfter
		) = 0;

	protected:
		ImmediateDeviceContext(GraphicsID aID);

	private:
	};
}