#include "Includes/JFA.hlsli"
#include "Includes/Buffers/JFABuffer.hlsli"

Texture2D Texture : register(t0);

SamplerState Sampler : register(s0);

JFA_PS_Out main(JFA_PS_In input)
{
    JFA_PS_Out output;
    output.Result = Texture.Sample(Sampler, input.UV);

    return output;
}