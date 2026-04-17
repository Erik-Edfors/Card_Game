#include "Includes/JFA.hlsli"
#include "Includes/Buffers/JFABuffer.hlsli"

Texture2D Texture : register(t0);

SamplerState Sampler : register(s0);

static const float2 Offsets[9] = {
    float2(0.f, 0.f),
    float2(0.f, 1.f),
    float2(1.f, 0.f),
    float2(1.f, 1.f),
    float2(0.f, -1.f),
    float2(-1.f, 0.f),
    float2(-1.f, -1.f),
    float2(-1.f, 1.f),
    float2(1.f, -1.f),
};

JFA_PS_Out main(JFA_PS_In input)
{
    JFA_PS_Out output;
    output.Result = 0.f;
    
    for (int i = 0; i < 9; i++)
    {
        output.Result += Texture.Sample(Sampler, input.UV + Offsets[i] * JFAB_StepDistance);
    }
    output.Result *= 1.f / 9.f;

    return output;
}