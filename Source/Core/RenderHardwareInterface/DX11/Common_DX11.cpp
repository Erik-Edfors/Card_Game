#include "RenderHardwareInterface.pch.h"
#include "Common_DX11.h"

#undef FILE_CATEGORY
#undef CLASS_CATEGORY
#define FILE_CATEGORY "Common_DX11.cpp"
#define CLASS_CATEGORY FILE_CATEGORY"->"

DXGI_FORMAT graphics::dx11::GetDXGIFormat(GRAPHICS_FORMAT aFormat)
{
    return static_cast<DXGI_FORMAT>(aFormat);
}

D3D11_INPUT_CLASSIFICATION graphics::dx11::GetInputClassification(GRAPHICS_INPUT_CLASS aInputClass)
{
#define LOG_CATEGORY CLASS_CATEGORY"::GetInputClassification"

    switch (aInputClass)
    {
    case GRAPHICS_INPUT_PER_VERTEX: return D3D11_INPUT_PER_VERTEX_DATA;
    case GRAPHICS_INPUT_PER_INSTANCE: return D3D11_INPUT_PER_INSTANCE_DATA;
    default:
        LOG_WARNING(LOG_CATEGORY, "Failed to convert " STRINGIFY(aInputClass) " '%s', defaulting to " STRINGIFY(D3D11_INPUT_PER_VERTEX_DATA) "!", static_cast<int>(aInputClass));
        return D3D11_INPUT_PER_VERTEX_DATA;
    }

#undef LOG_CATEGORY
}

D3D11_USAGE graphics::dx11::GetD3D11Usage(GRAPHICS_USAGE aUsage)
{
#define LOG_CATEGORY CLASS_CATEGORY"::GetD3D11Usage"

    switch (aUsage)
    {
    case GRAPHICS_USAGE_DEFAULT:                return D3D11_USAGE_DEFAULT;
    case GRAPHICS_USAGE_IMMUTABLE:              return D3D11_USAGE_IMMUTABLE;
    case GRAPHICS_USAGE_DYNAMIC:                return D3D11_USAGE_DYNAMIC;
    case GRAPHICS_USAGE_STAGING:                return D3D11_USAGE_STAGING;
    default:
        LOG_WARNING(LOG_CATEGORY, "Failed to convert aGraphicsUsage '%s', defaulting to " STRINGIFY(D3D11_USAGE_DEFAULT) "!", static_cast<int>(aUsage));
        return D3D11_USAGE_DEFAULT;
    }

#undef LOG_CATEGORY
}

D3D11_BIND_FLAG graphics::dx11::GetD3D11BindFlags(GRAPHICS_BIND_FLAGS aTextureFlags)
{
    int flags = NULL;

    if (aTextureFlags & GRAPHICS_BIND_SHADER_RESOURCE)
    {
        flags |= D3D11_BIND_SHADER_RESOURCE;
    }
    if (aTextureFlags & GRAPHICS_BIND_RENDER_TARGET)
    {
        flags |= D3D11_BIND_RENDER_TARGET;
    }
    if (aTextureFlags & GRAPHICS_BIND_DEPTH_STENCIL)
    {
        flags |= D3D11_BIND_DEPTH_STENCIL;
    }

    return (D3D11_BIND_FLAG)flags;
}

D3D11_RESOURCE_MISC_FLAG graphics::dx11::GetD3D11ResourceMiscFlag(GRAPHICS_RESOURCE_FLAGS aResourceFlags)
{
    return static_cast<D3D11_RESOURCE_MISC_FLAG>(aResourceFlags);
}

//D3D11_RESOURCE_MISC_FLAG graphics::dx11::ResourceMiscFlags(GRAPHICS_RESOURCE_MISC_FLAG aTextureFlags)
//{
//    return static_cast<D3D11_RESOURCE_MISC_FLAG>(aTextureFlags);
//}

D3D11_CPU_ACCESS_FLAG graphics::dx11::GetD3D11CPUAccessFlag(GRAPHICS_CPU_ACCESS_FLAGS aCPUAccessFlags)
{
    int flags = NULL;

    if (aCPUAccessFlags & GRAPHICS_CPU_ACCESS_WRITE)
    {
        flags |= D3D11_CPU_ACCESS_WRITE;
    }
    if (aCPUAccessFlags & GRAPHICS_CPU_ACCESS_READ)
    {
        flags |= D3D11_CPU_ACCESS_READ;
    }

    return (D3D11_CPU_ACCESS_FLAG)flags;
}

D3D11_DSV_FLAG graphics::dx11::GetD3D11DSVFlag(GRAPHICS_DEPTH_STENCIL_FLAGS aDepthStencilFlags)
{
    int flags = NULL;

    if (aDepthStencilFlags & GRAPHICS_DEPTH_STENCIL_DEPTH_READ_ONLY)
    {
        flags = D3D11_DSV_READ_ONLY_DEPTH;
    }
    if (aDepthStencilFlags & GRAPHICS_DEPTH_STENCIL_STENCIL_READ_ONLY)
    {
        flags = D3D11_DSV_READ_ONLY_STENCIL;
    }

    return static_cast<D3D11_DSV_FLAG>(flags);
}

D3D11_PRIMITIVE_TOPOLOGY graphics::dx11::GetD3D11PrimitiveTopology(GRAPHICS_TOPOLOGY aTopology)
{
    return static_cast<D3D11_PRIMITIVE_TOPOLOGY>(aTopology);
}

D3D11_TEXTURE_ADDRESS_MODE graphics::dx11::GetD3D11TextureAddressMode(GRAPHICS_TEXTURE_ADDRESS_MODE aAddressMode)
{
    return static_cast<D3D11_TEXTURE_ADDRESS_MODE>(aAddressMode);
}

D3D11_COMPARISON_FUNC graphics::dx11::GetD3D11ComparisonFunction(GRAPHICS_COMPARISON aComparison)
{
    return static_cast<D3D11_COMPARISON_FUNC>(aComparison);
}

D3D11_FILTER graphics::dx11::GetD3D11Filter(GRAPHICS_SAMPLER_FILTER aFilter)
{
    return static_cast<D3D11_FILTER>(aFilter);
}

D3D11_BLEND graphics::dx11::GetD3D11Blend(GRAPHICS_BLEND aBlend)
{
    return static_cast<D3D11_BLEND>(aBlend);
}

D3D11_BLEND_OP graphics::dx11::GetD3D11BlendOp(GRAPHICS_BLEND_OPERATION aBlendOperation)
{
    return static_cast<D3D11_BLEND_OP>(aBlendOperation);
}

D3D11_COLOR_WRITE_ENABLE graphics::dx11::GetD3D11ColorWriteEnable(GRAPHICS_COLOR_WRITE_ENABLE aColorWriteEnable)
{
    return static_cast<D3D11_COLOR_WRITE_ENABLE>(aColorWriteEnable);
}

D3D11_MAP graphics::dx11::GetD3D11Map(GRAPHICS_MAP aMapType)
{
    return static_cast<D3D11_MAP>(aMapType);
}

D3D11_MAP_FLAG graphics::dx11::GetD3D11MapFlag(GRAPHICS_MAP_FLAGS aMapFlags)
{
    return static_cast<D3D11_MAP_FLAG>(aMapFlags);
}

D3D11_SRV_DIMENSION graphics::dx11::GetD3D11SRVDimension(GRAPHICS_SHADER_RESOURCE_VIEW_DIMENSION aShaderResourceViewDimension)
{
    return static_cast<D3D11_SRV_DIMENSION>(aShaderResourceViewDimension);
}

D3D11_RTV_DIMENSION graphics::dx11::GetD3D11RTVDimension(GRAPHICS_RENDER_TARGET_VIEW_DIMENSION aRenderTargetViewDimension)
{
    return static_cast<D3D11_RTV_DIMENSION>(aRenderTargetViewDimension);
}

D3D11_DSV_DIMENSION graphics::dx11::GetD3D11DSVDimension(GRAPHICS_DEPTH_STENCIL_VIEW_DIMENSION aDepthStencilViewDimension)
{
    return static_cast<D3D11_DSV_DIMENSION>(aDepthStencilViewDimension);
}

D3D11_FILL_MODE graphics::dx11::GetD3D11FillMode(GRAPHICS_FILL_MODE aFillMode)
{
    return static_cast<D3D11_FILL_MODE>(aFillMode);
}

D3D11_CULL_MODE graphics::dx11::GetD3D11CullMode(GRAPHICS_CULL_MODE aCullMode)
{
    return static_cast<D3D11_CULL_MODE>(aCullMode);
}

BOOL graphics::dx11::IsWindingOrderCounterClockwise(GRAPHICS_WINDING_ORDER aWindingOrder)
{
    return aWindingOrder;
}

D3D11_DEPTH_WRITE_MASK graphics::dx11::GetD3D11DepthWriteMask(GRAPHICS_DEPTH_WRITE_MASK aWriteMask)
{
    return static_cast<D3D11_DEPTH_WRITE_MASK>(aWriteMask);
}

D3D11_STENCIL_OP graphics::dx11::GetD3D11StencilOp(GRAPHICS_STENCIL_OPERATION aStencilOperation)
{
    return static_cast<D3D11_STENCIL_OP>(aStencilOperation);
}

graphics::GRAPHICS_FORMAT graphics::dx11::GetGraphicsFormat(DXGI_FORMAT aFormat)
{
    return static_cast<graphics::GRAPHICS_FORMAT>(aFormat);
}

graphics::GRAPHICS_BIND_FLAGS graphics::dx11::GetGraphicsTextureFlags(D3D11_BIND_FLAG aBindFlag)
{
    graphics::GRAPHICS_BIND_FLAGS flags = graphics::GRAPHICS_BIND_NONE;

    if (aBindFlag & D3D11_BIND_SHADER_RESOURCE)
    {
        flags |= GRAPHICS_BIND_SHADER_RESOURCE;
    }
    if (aBindFlag & D3D11_BIND_RENDER_TARGET)
    {
        flags |= GRAPHICS_BIND_RENDER_TARGET;
    }
    if (aBindFlag & D3D11_BIND_DEPTH_STENCIL )
    {
        flags |= GRAPHICS_BIND_DEPTH_STENCIL;
    }

    return flags;
}

graphics::GRAPHICS_RESOURCE_FLAGS graphics::dx11::GetGraphicsResourceFlags(D3D11_RESOURCE_MISC_FLAG aResourceMiscFlags)
{
    return static_cast<graphics::GRAPHICS_RESOURCE_FLAGS>(aResourceMiscFlags);
}

graphics::GRAPHICS_USAGE graphics::dx11::GetGraphicsUsage(D3D11_USAGE aUsage)
{
#define LOG_CATEGORY CLASS_CATEGORY"::GetGraphicsUsage"

    switch (aUsage)
    {
    case D3D11_USAGE_DEFAULT:                return GRAPHICS_USAGE_DEFAULT;
    case D3D11_USAGE_IMMUTABLE:              return GRAPHICS_USAGE_IMMUTABLE;
    case D3D11_USAGE_DYNAMIC:                return GRAPHICS_USAGE_DYNAMIC;
    case D3D11_USAGE_STAGING:                return GRAPHICS_USAGE_STAGING;
    default:
        LOG_WARNING(LOG_CATEGORY, "Failed to convert aUsage '%s', defaulting to " STRINGIFY(GRAPHICS_USAGE_DEFAULT) "!", static_cast<int>(aUsage));
        return GRAPHICS_USAGE_DEFAULT;
    }

#undef LOG_CATEGORY
}

graphics::GRAPHICS_CPU_ACCESS_FLAGS graphics::dx11::GetGraphicsCPUAccessFlags(D3D11_CPU_ACCESS_FLAG aCPUAccessFlags)
{
    GRAPHICS_CPU_ACCESS_FLAGS flags = GRAPHICS_CPU_ACCESS_NONE;

    if (aCPUAccessFlags & D3D11_CPU_ACCESS_WRITE)
    {
        flags |= GRAPHICS_CPU_ACCESS_WRITE;
    }
    if (aCPUAccessFlags & D3D11_CPU_ACCESS_READ)
    {
        flags |= GRAPHICS_CPU_ACCESS_READ;
    }

    return flags;
}
