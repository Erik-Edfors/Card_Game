#include "Includes/JFA.hlsli"

[maxvertexcount(4)]
void main(
	point JFA_GS_In input[1], 
	inout TriangleStream<JFA_GS_Out> output
)
{
    const JFA_GS_In inputElement = input[0];
    
    JFA_GS_Out topLeft;
    topLeft.Position = float4(inputElement.Rectangle[0], inputElement.Rectangle[0], 0.f, 1.f);
    topLeft.UV = float2(inputElement.Rectangle[0], inputElement.Rectangle[0]);
	
    JFA_GS_Out topRight;
    topRight.Position = float4(inputElement.Rectangle[2], inputElement.Rectangle[0], 0.f, 1.f);
    topRight.UV = float2(inputElement.Rectangle[2], inputElement.Rectangle[0]);
    
    JFA_GS_Out bottomLeft;
    bottomLeft.Position = float4(inputElement.Rectangle[0], inputElement.Rectangle[3], 0.f, 1.f);
    bottomLeft.UV = float2(inputElement.Rectangle[0], inputElement.Rectangle[3]);
    
    JFA_GS_Out bottomRight;
    bottomRight.Position = float4(inputElement.Rectangle[2], inputElement.Rectangle[3], 0.f, 1.f);
    bottomRight.UV = float2(inputElement.Rectangle[2], inputElement.Rectangle[3]);
	    
    output.Append(topRight);
    output.Append(topLeft);
    output.Append(bottomRight);
    output.Append(bottomLeft);
}