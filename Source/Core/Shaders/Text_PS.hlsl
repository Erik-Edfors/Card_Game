#include "Includes/Text.hlsli"

SamplerState Sampler : register(s0);

Texture2D Atlas : register(t0);

Text_PS_Out main(Text_PS_In input)
{
    Text_PS_Out output;
    output.Color = Atlas.Sample(Sampler, input.UV);
	return output;
}