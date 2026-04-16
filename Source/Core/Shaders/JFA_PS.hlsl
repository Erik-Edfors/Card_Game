#include "Includes/JFA.hlsli"
#include "Includes/Buffers/JFABuffer.hlsli"

Texture2D Texture : register(t0);

SamplerState Sampler : register(s0);

static const float2 Offsets[9] = {
    float2(0.f, 0.f),
    float2(-1.f, 0.f),
    float2(0.f, -1.f),
    float2(1.f, 0.f),
    float2(0.f, 1.f),
    float2(-1.f, -1.f),
    float2(-1.f, 1.f),
    float2(1.f, -1.f),
    float2(1.f, 1.f),
};

JFA_PS_Out main(JFA_PS_In input)
{   
    JFA_PS_Out output;

    output.Result = Texture.Sample(Sampler, input.UV);
    	
    float minDistance = 1e10f;

    [unroll]
    for (int i = 0; i < 9; i++)
    {
        const float4 sample = Texture.Sample(Sampler, input.UV + Offsets[i] * JFAB_StepDistance);
        
        const float2 sampleUV = sample.rg;
        const bool sampleInitialized = (length(sampleUV) != 0.f);
        
        const float dist = distance(input.UV, sampleUV) + (sampleInitialized ? 0.f : 1e12f);
        if (dist < minDistance)
        {
            minDistance = dist;
            output.Result = sample;
        }
    }
    
    return output;
}