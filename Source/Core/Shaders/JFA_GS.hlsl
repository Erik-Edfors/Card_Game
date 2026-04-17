#include "Includes/JFA.hlsli"

float4 ConvertToNDC(float4 aPosition)
{
    return float4((2.f * aPosition.xy) - float2(1.f, 1.f), aPosition.zw);
    //return aPosition;
}

[maxvertexcount(4)]
void main(
	point JFA_GS_In input[1], 
	inout TriangleStream<JFA_GS_Out> output
)
{
    const JFA_GS_In inputElement = input[0];
    
    JFA_GS_Out topLeft;
    topLeft.Position = ConvertToNDC(float4(inputElement.Rectangle[0], 1.f - inputElement.Rectangle[1], 0.f, 1.f));
    topLeft.UV = float2(inputElement.UVRectangle[0], inputElement.UVRectangle[1]);
	
    JFA_GS_Out topRight;
    topRight.Position = ConvertToNDC(float4(inputElement.Rectangle[2], 1.f - inputElement.Rectangle[1], 0.f, 1.f));
    topRight.UV = float2(inputElement.UVRectangle[2], inputElement.UVRectangle[1]);
    
    JFA_GS_Out bottomLeft;
    bottomLeft.Position = ConvertToNDC(float4(inputElement.Rectangle[0], 1.f - inputElement.Rectangle[3], 0.f, 1.f));
    bottomLeft.UV = float2(inputElement.UVRectangle[0], inputElement.UVRectangle[3]);
    
    JFA_GS_Out bottomRight;
    bottomRight.Position = ConvertToNDC(float4(inputElement.Rectangle[2], 1.f - inputElement.Rectangle[3], 0.f, 1.f));
    bottomRight.UV = float2(inputElement.UVRectangle[2], inputElement.UVRectangle[3]);
	    
    output.Append(topLeft);
    output.Append(topRight);
    output.Append(bottomLeft);
    output.Append(bottomRight);
}