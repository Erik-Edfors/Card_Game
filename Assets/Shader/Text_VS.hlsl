#include "Include/Text.hlsli"

Text_VS_Out main(Text_VS_In input)
{
    Text_VS_Out output;
    
    const float4x4 transform = mul(FB_Projection, mul(FB_View, OB_Transform));
    
    const float2 topLeft = input.Bounds.xz;
    const float2 topRight = input.Bounds.yz;
    const float2 bottomLeft = input.Bounds.xw;
    const float2 bottomRight = input.Bounds.yw;
    
    output.Corners[0] = mul(transform, float4(topLeft, 0.f, 1.f));
    output.Corners[1] = mul(transform, float4(topRight, 0.f, 1.f));
    output.Corners[2] = mul(transform, float4(bottomLeft, 0.f, 1.f));
    output.Corners[3] = mul(transform, float4(bottomRight, 0.f, 1.f));
    
    output.UVRect = input.UVRect;
    
	return output;
}