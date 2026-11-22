cbuffer FrameBuffer : register(b0)
{
    float4x4 FB_View;
    float4x4 FB_Projection;
    float2 FB_TargetSize;

    uint2 FB_padding;
};

cbuffer ObjectBuffer : register(b1)
{
    float4x4 OB_Transform;
    float4 OB_Tint;
    float4 OB_AtlasRect;
    float2 OB_Origin;
    uint OB_ID;

    uint OB_padding;
};

struct Sprite_VS_Out
{
    float4 Position : SV_POSITION;
    float2 UV : TEXCOORD;
};

struct Sprite_PS_In
{
    float4 Position : SV_POSITION;
    float2 UV : TEXCOORD;
};

struct Sprite_PS_Out
{
    float4 Color : SV_TARGET0;
    uint ID : SV_TARGET1;
};