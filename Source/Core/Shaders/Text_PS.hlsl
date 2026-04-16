#include "Includes/Text.hlsli"

SamplerState Sampler : register(s0);

Texture2D Atlas : register(t0);

Text_PS_Out main(Text_PS_In input)
{
    const float d = distance(input.LocalUV, Atlas.Sample(Sampler, input.UV).rg);
    
    Text_PS_Out output;
    float factor = (d > 0.f && d < 0.01f) ? 1.f : 0.f;
    output.Color = float4(1.f, 1.f, 1.f, 1.f) * factor;
	return output;
}