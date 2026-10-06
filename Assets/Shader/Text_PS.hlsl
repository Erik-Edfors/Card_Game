#include "Include/Text.hlsli"

SamplerState Sampler : register(s0);

Texture2D Atlas : register(t0);

float median(float r, float g, float b) 
{
    return max(min(r, g), min(max(r, g), b));
}

Text_PS_Out main(Text_PS_In input)
{
    Text_PS_Out output;
    float4 s = Atlas.Sample(Sampler, input.UV);
    float sd = median(s.r, s.g, s.b);

    const float atlasBorderThickness = OB_AtlasEmSize * OB_BorderThickness;

    const float distanceToGlyph = OB_SDLength * (0.5f - sd);
    const float distanceToBorder = distanceToGlyph - atlasBorderThickness;
    const float blurDistance = 2.f * OB_SDLength / OB_AtlasEmSize;     //TODO: fix Render Target size dependant.

    clip(-distanceToBorder);

    const float colorFactor = smoothstep(0.f, blurDistance, distanceToGlyph);

    const float4 color = OB_Color + colorFactor * (OB_BorderColor - OB_Color);

    float opacity = 1.f - smoothstep(0.f, blurDistance, distanceToBorder + blurDistance);

    output.Color = float4(color.rgb, color.a * opacity);

    return output;
    //const float d = distance(input.LocalUV, Atlas.Sample(Sampler, input.UV).rg);
    
    //Text_PS_Out output;
    ////output.Color = float4(d, 0.f, 0.f, 1.f);
    //float factor = 1.f - smoothstep(0.05f, 0.1f, d);
    //output.Color = float4(1.f, 1.f, 1.f, 1.f) * factor;
    //return output;
}