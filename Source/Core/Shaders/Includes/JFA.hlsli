struct JFA_VS_In
{
    float4 Rectangle : RECTANGLE;
};

struct JFA_VS_Out
{
    float4 Rectangle : RECTANGLE;
};

struct JFA_GS_In
{
    float4 Rectangle : RECTANGLE;
};

struct JFA_GS_Out
{
    float4 Position : SV_POSITION;
    float2 UV : TEXCOORD;
};

struct JFA_PS_In
{
    float2 UV : TEXCOORD;
};

struct JFA_PS_Out
{
    float4 Result : SV_TARGET;
};