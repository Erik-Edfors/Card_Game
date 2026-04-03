#pragma once
#include <Core/RenderHardwareInterface/GraphicsDefs.h>

graphics::GRAPHICS_FILL_MODE GraphicsFillModeFromString(const char* aString);

graphics::GRAPHICS_CULL_MODE GraphicsCullModeFromString(const char* aString);

graphics::GRAPHICS_WINDING_ORDER GraphicsWindingOrderFromString(const char* aString);

graphics::GRAPHICS_TOPOLOGY GraphicsTopologyFromString(const char* aString);

graphics::GRAPHICS_FORMAT GraphicsFormatFromString(const char* aString);

graphics::GRAPHICS_BLEND GraphicsBlendFromString(const char* aString);

graphics::GRAPHICS_BLEND_OPERATION GraphicsBlendOperationFromString(const char* aString);

graphics::GRAPHICS_DEPTH_STENCIL_FLAGS GraphicsDepthStencilFlagFromString(const char* aString);

graphics::GRAPHICS_INPUT_CLASS GraphicsInputClassFromString(const char* aString);

graphics::GRAPHICS_SAMPLER_FILTER GraphicsSamplerFilterFromString(const char* aString);

graphics::GRAPHICS_TEXTURE_ADDRESS_MODE GraphicsTextureAddressModeFromString(const char* aString);

graphics::GRAPHICS_COMPARISON GraphicsComparisonFromString(const char* aString);

graphics::GRAPHICS_SHADER_RESOURCE_VIEW_DIMENSION GraphicsShaderResourceViewDimensionFromString(const char* aString);
graphics::GRAPHICS_RENDER_TARGET_VIEW_DIMENSION GraphicsRenderTargetViewDimensionFromString(const char* aString);
graphics::GRAPHICS_DEPTH_STENCIL_VIEW_DIMENSION GraphicsDepthStencilViewDimensionFromString(const char* aString);

graphics::GRAPHICS_BIND_FLAGS GraphicsBindFlagFromString(const char* aString);

graphics::GRAPHICS_RESOURCE_FLAGS GraphicsResourceFlagFromString(const char* aString);

graphics::GRAPHICS_USAGE GraphicsUsageFromString(const char* aString);

graphics::GRAPHICS_CPU_ACCESS_FLAGS GraphicsCPUAccessFlagFromString(const char* aString);

graphics::GRAPHICS_SHADER_STAGE_FLAGS GraphicsShaderStageFlagFromString(const char* aString);

graphics::GRAPHICS_DEPTH_WRITE_MASK GraphicsDepthWriteMaskFromString(const char* aString);

graphics::GRAPHICS_STENCIL_OPERATION GraphicsStencilOperationFromString(const char* aString);
