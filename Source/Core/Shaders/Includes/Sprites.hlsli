
struct Sprite_VS_In
{
    float2 Position : Position;
    float2 Size : Size;
    float4 UVRect : UVRect;
    float2 Pivot : Pivot;
    float Rotation : Rotation;
};

struct Sprite_VS_Out
{
    float2 Rectangle[4] : Rectangle;
    float4 UVRect : UVRect;
};

struct Sprite_GS_In
{
    float2 Rectangle[4] : Rectangle;
    float4 UVRect : UVRect;
};

struct Sprite_GS_Out
{
    float2 Position : SV_Position;
    float2 UV : TEXCOORD;
};

struct Sprite_PS_In
{
    float2 Position : SV_Position;
    float2 UV : TEXCOORD;
};

struct Sprite_PS_Out
{
    float Color : SV_Target;
};