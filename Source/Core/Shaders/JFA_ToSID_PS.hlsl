#include "Includes/JFA.hlsli"

Texture2D Texture : register(t0);

SamplerState Sampler : register(s0);

JFA_PS_Out main(JFA_PS_In input)
{
    JFA_PS_Out output;
    output.Result = float4(distance(input.UV, Texture.Sample(Sampler, input.UV).rg), 0.f, 0.f, 0.f);
    return output;
}