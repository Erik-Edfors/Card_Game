
struct Text_VS_In
{
    float4 Bounds : BOUNDS;
    float4 UVRect : UVRECT;
};

struct Text_VS_Out
{
    float4 Corners[4] : CORNERS;
    float4 UVRect : UVRECT;
};

struct Text_GS_In
{
    float4 Corners[4] : CORNERS;
    float4 UVRect : UVRECT;
};

struct Text_GS_Out
{
    float4 Position : SV_POSITION;
    float2 UV : TEXCOORD;
    float2 LocalUV : LOCALUV;
};

struct Text_PS_In
{
    float4 Position : SV_POSITION;
    float2 UV : TEXCOORD;
    float2 LocalUV : LOCALUV;
};

struct Text_PS_Out
{
    float4 Color : SV_TARGET;
};