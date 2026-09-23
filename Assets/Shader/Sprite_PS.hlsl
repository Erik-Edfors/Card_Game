#include "Include/Sprite.hlsli"

SamplerState Sampler : register(s0);
Texture2D Atlas : register(t0);

Sprite_PS_Out main(Sprite_PS_In input)
{
    Sprite_PS_Out output;
    output.Color = Atlas.Sample(Sampler, input.UV) * OB_Tint;
    return output;
}