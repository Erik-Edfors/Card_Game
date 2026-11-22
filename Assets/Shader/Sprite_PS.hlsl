#include "Include/Sprite.hlsli"

SamplerState Sampler : register(s0);
Texture2D Atlas : register(t0);

Sprite_PS_Out main(Sprite_PS_In input)
{
    Sprite_PS_Out output;
    output.Color = Atlas.Sample(Sampler, input.UV) * OB_Tint;
    clip(output.Color.a - 0.05f);
    output.ID = OB_ID;
    return output;
}