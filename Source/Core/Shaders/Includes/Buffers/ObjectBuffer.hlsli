
cbuffer ObjectBuffer : register(b1)
{
    float4x4 OB_World;
    uint OB_ID;
    uint3 OB_Padding;
}