#include "Includes/Sprites.hlsli"

[maxvertexcount(4)]
void main(
	point Sprite_GS_In input[1],
	inout TriangleStream<Sprite_GS_Out> output
)
{
    const Sprite_GS_In sprite = input[0];
    
    Sprite_GS_Out topLeft;
    topLeft.Position = float4(sprite.Corners[0], 0.f, 1.f);
    topLeft.UV = float2(sprite.UVRect[0], sprite.UVRect[0]);
	
    Sprite_GS_Out topRight;
    topRight.Position = float4(sprite.Corners[1], 0.f, 1.f);
    topRight.UV = float2(sprite.UVRect[2], sprite.UVRect[0]);
    
    Sprite_GS_Out bottomLeft;
    bottomLeft.Position = float4(sprite.Corners[2], 0.f, 1.f);
    bottomLeft.UV = float2(sprite.UVRect[0], sprite.UVRect[3]);
    
    Sprite_GS_Out bottomRight;
    bottomRight.Position = float4(sprite.Corners[3], 0.f, 1.f);
    bottomRight.UV = float2(sprite.UVRect[2], sprite.UVRect[3]);
	    
    output.Append(topRight);
    output.Append(topLeft);
    output.Append(bottomRight);
    output.Append(bottomLeft);
}