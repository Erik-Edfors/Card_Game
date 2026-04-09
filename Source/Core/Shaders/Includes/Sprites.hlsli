
struct Sprite_VS_In
{
    float2 Position : POSITION;
    float2 Size : SIZE;
    float4 UVRect : UVRECT;
    float2 Pivot : PIVOT;
    float Rotation : ROTATION;
};

struct Sprite_VS_Out
{
    float2 Corners[4] : CORNERS;
    float4 UVRect : UVRECT;
};

struct Sprite_GS_In
{
    float2 Corners[4] : CORNERS;
    float4 UVRect : UVRECT;
};

struct Sprite_GS_Out
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
    float4 Color : SV_TARGET;
};