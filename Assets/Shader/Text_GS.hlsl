#include "Include/Text.hlsli"

[maxvertexcount(4)]
void main(
	point Text_GS_In input[1], 
	inout TriangleStream<Text_GS_Out> output
)
{
    const Text_GS_In glyph = input[0];
    
    Text_GS_Out topLeft;
    topLeft.Position = glyph.Corners[0];
    topLeft.UV = float2(glyph.UVRect[0], glyph.UVRect[2]);
    //topLeft.LocalUV = float2(0.f, 1.f);
	
    Text_GS_Out topRight;
    topRight.Position = glyph.Corners[1];
    topRight.UV = float2(glyph.UVRect[1], glyph.UVRect[2]);
    //topRight.LocalUV = float2(1.f, 1.f);
    
    Text_GS_Out bottomLeft;
    bottomLeft.Position = glyph.Corners[2];
    bottomLeft.UV = float2(glyph.UVRect[0], glyph.UVRect[3]);
    //bottomLeft.LocalUV = float2(0.f, 0.f);
    
    Text_GS_Out bottomRight;
    bottomRight.Position = glyph.Corners[3];
    bottomRight.UV = float2(glyph.UVRect[1], glyph.UVRect[3]);
    //bottomRight.LocalUV = float2(1.f, 0.f);
	    
    output.Append(topLeft);
    output.Append(topRight);
    output.Append(bottomLeft);
    output.Append(bottomRight);
}