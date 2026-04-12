#include "Includes/Text.hlsli"

[maxvertexcount(6)]
void main(
	point Text_GS_In input[1], 
	inout TriangleStream<Text_GS_Out> output
)
{
    const Text_GS_In glyph = input[0];
    
    Text_GS_Out topLeft;
    topLeft.Position = glyph.Corners[0];
    topLeft.UV = float2(glyph.UVRect[0], glyph.UVRect[0]);
	
    Text_GS_Out topRight;
    topRight.Position = glyph.Corners[1];
    topRight.UV = float2(glyph.UVRect[2], glyph.UVRect[0]);
    
    Text_GS_Out bottomLeft;
    bottomLeft.Position = glyph.Corners[2];
    bottomLeft.UV = float2(glyph.UVRect[0], glyph.UVRect[3]);
    
    Text_GS_Out bottomRight;
    bottomRight.Position = glyph.Corners[3];
    bottomRight.UV = float2(glyph.UVRect[2], glyph.UVRect[3]);
	    
    output.Append(topLeft);
    output.Append(topRight);
    output.Append(bottomRight);
    
    output.Append(topLeft);
    output.Append(bottomRight);
    output.Append(bottomLeft);
}