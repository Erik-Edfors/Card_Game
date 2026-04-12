#include "Includes/Text.hlsli"
#include "Includes/Buffers/FrameBuffer.hlsli"
#include "Includes/Buffers/ObjectBuffer.hlsli"

Text_VS_Out main(Text_VS_In input)
{
    Text_VS_Out output;
    
    const float4x4 transform = mul(OB_World, mul(FB_View, FB_Projection));
    
    const float2 topLeft = input.Bounds.xy;
    const float2 topRight = input.Bounds.zy;
    const float2 bottomLeft = input.Bounds.xw;
    const float2 bottomRight = input.Bounds.zw;
    
    output.Corners[0] = mul(transform, float4(topLeft, 0.f, 1.f));
    output.Corners[1] = mul(transform, float4(topRight, 0.f, 1.f));
    output.Corners[2] = mul(transform, float4(bottomLeft, 0.f, 1.f));
    output.Corners[3] = mul(transform, float4(bottomRight, 0.f, 1.f));
    
    output.UVRect = input.UVRect;
    
	return output;
}