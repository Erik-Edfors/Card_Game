#include "Include/Sprite.hlsli"

static const uint Indices[] = {
    0, 2, 1,
    1, 2, 3
};

static const uint2 UVIndices[] = {
    uint2(0, 3),
    uint2(2, 3),
    uint2(0, 1),
    uint2(2, 1),
};

static const float4 Vertices[] = {
    float4(0.f, 0.f, 0.f, 1.f),
    float4(1.f, 0.f, 0.f, 1.f),
    float4(0.f, 1.f, 0.f, 1.f),
    float4(1.f, 1.f, 0.f, 1.f)
};

Sprite_VS_Out main(uint aVertexID : SV_VertexID)
{
    Sprite_VS_Out output;

    const float4x4 transform = mul(FB_Projection, mul(FB_View, OB_Transform));

    const uint index = Indices[aVertexID];
    const uint2 rectArrayIndices = UVIndices[index];
    const float rectArray[4] = (float[4])OB_AtlasRect;

    output.Position = mul(transform, Vertices[index] - float4(OB_Origin, 0.f, 0.f));

    output.UV.x = rectArray[rectArrayIndices.x];
    output.UV.y = rectArray[rectArrayIndices.y];

    return output;
}