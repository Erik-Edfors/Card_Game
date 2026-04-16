struct JFA_VS_In
{
    float4 Rectangle : RECTANGLE;
    float4 UVRectangle : UVRECTANGLE;
};

struct JFA_VS_Out
{
    float4 Rectangle : RECTANGLE;
    float4 UVRectangle : UVRECTANGLE;
};

struct JFA_GS_In
{
    float4 Rectangle : RECTANGLE;
    float4 UVRectangle : UVRECTANGLE;
};

struct JFA_GS_Out
{
    float4 Position : SV_POSITION;
    float2 UV : TEXCOORD;
};

struct JFA_PS_In
{
    float4 Position : SV_POSITION;
    float2 UV : TEXCOORD;
};

struct JFA_PS_Out
{
    float4 Result : SV_TARGET;
};