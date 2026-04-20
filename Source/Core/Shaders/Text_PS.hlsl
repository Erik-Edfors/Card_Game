#include "Includes/Text.hlsli"

SamplerState Sampler : register(s0);

Texture2D Atlas : register(t0);

Text_PS_Out main(Text_PS_In input)
{
    Text_PS_Out output;
    output.Color = Atlas.Sample(Sampler, input.UV);
    return output;
    //const float d = distance(input.LocalUV, Atlas.Sample(Sampler, input.UV).rg);
    
    //Text_PS_Out output;
    ////output.Color = float4(d, 0.f, 0.f, 1.f);
    //float factor = 1.f - smoothstep(0.05f, 0.1f, d);
    //output.Color = float4(1.f, 1.f, 1.f, 1.f) * factor;
    //return output;
}