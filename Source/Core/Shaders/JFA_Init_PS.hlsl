#include "Includes/JFA.hlsli"

Texture2D Texture : register(t0);

SamplerState Sampler : register(s0);

JFA_PS_Out main(JFA_PS_In input)
{
    JFA_PS_Out output;
    
    if (length(Texture.Sample(Sampler, input.UV)) != 0.f)
    {
        output.Result = float4(input.UV, 1.f, 0.f);
    }
    else
    {
        output.Result = float4(0.f, 0.f, 0.f, 0.f);
    }

    return output;
}