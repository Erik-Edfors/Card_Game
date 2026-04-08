#include "Includes/Sprites.hlsli"
#include "Includes/Buffers/FrameBuffer.hlsli"

Sprite_VS_Out main(Sprite_VS_In input)
{
    Sprite_VS_Out output;
    output.UVRect = input.UVRect;
    
    //TODO: Calculate position around pivots.
    output.Rectangle[0] = input.Position;
    output.Rectangle[1] = input.Position + float2(input.Size.x, 0.f);
    output.Rectangle[2] = input.Position + float2(0.f, input.Size.y);
    output.Rectangle[3] = input.Position + float2(input.Size.x, input.Size.y);
        
    [unroll]
    for (unsigned int i = 0; i < 4; i++)
    {
        output.Rectangle[i] = mul(FB_View, float4(output.Rectangle[i], 0.f, 1.f)).xy;
        output.Rectangle[i] = mul(FB_View, float4(output.Rectangle[i], 0.f, 1.f)).xy;
    }
    
    return output;
}