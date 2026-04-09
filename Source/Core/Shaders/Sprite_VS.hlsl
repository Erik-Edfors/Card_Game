#include "Includes/Sprites.hlsli"
#include "Includes/Buffers/FrameBuffer.hlsli"

Sprite_VS_Out main(Sprite_VS_In input)
{
    Sprite_VS_Out output;
    output.UVRect = input.UVRect;
    
    const float2 pivot = input.Pivot * input.Size;
    
    output.Corners[0] = input.Position - pivot;
    output.Corners[1] = input.Position + float2(input.Size.x, 0.f) - pivot;
    output.Corners[2] = input.Position + float2(0.f, input.Size.y) - pivot;
    output.Corners[3] = input.Position + float2(input.Size.x, input.Size.y) - pivot;
    
    const float4x4 rotation = float4x4(
        cos(input.Rotation), sin(input.Rotation), 0.f, 0.f,
        -sin(input.Rotation), cos(input.Rotation), 0.f, 0.f,
        0.f, 0.f, 0.f, 0.f,
        0.f, 0.f, 0.f, 1.f
    );
    
    const float4x4 transform = mul(rotation, mul(FB_View, FB_Projection));
    
    [unroll]
    for (unsigned int i = 0; i < 4; i++)
    {
        output.Corners[i] = mul(transform, float4(output.Corners[i], 0.f, 1.f)).xy;
    }
    
    return output;
}