#include "Includes/Sprites.hlsli"

SamplerState Sampler : register(s0);

Texture2D Atlas : register(t0);

float4 main(Sprite_PS_In input) : SV_TARGET
{
    return Atlas.Sample(Sampler, input.UV);
}