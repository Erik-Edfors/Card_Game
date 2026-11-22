#include "Include/Text.hlsli"

SamplerState Sampler : register(s0);

Texture2D Atlas : register(t0);

float2 GetSize(Texture2D aTexture)
{
    float width, height;
    aTexture.GetDimensions(width, height);
    return float2(width, height);
}

float Median(float r, float g, float b) 
{
    return max(min(r, g), min(max(r, g), b));
}

float2 Sqr(float2 x)
{
    return x * x;
}

float GetScreenPxRange(float2 uv) {
    float2 unitRange = float2(OB_SDLength, OB_SDLength) / GetSize(Atlas);
    float2 screenTexSize = 1.f / sqrt(Sqr(ddx(uv)) + Sqr(ddy(uv)));
    return max(0.5f * dot(unitRange, screenTexSize), 1.f);
}

Text_PS_Out main(Text_PS_In input)
{
    Text_PS_Out output;

    const float3 msd = Atlas.Sample(Sampler, input.UV).rgb;
    const float sd = Median(msd.r, msd.g, msd.b);
    const float screenRange = GetScreenPxRange(input.UV);
    const float screenGlyphDistance = screenRange*(sd - 0.5f);
    const float screenBorderDistance = screenRange*(sd - 0.5f + OB_AtlasEmSize * OB_BorderThickness / OB_SDLength);
    const float glyphOpacity = clamp(screenGlyphDistance + 0.5f, 0.f, 1.f);
    const float borderOpacity = (1.f - glyphOpacity) * clamp(screenBorderDistance + 0.5f, 0.f, 1.f);

    clip(glyphOpacity + borderOpacity - 0.01f);

    output.Color = OB_Color * glyphOpacity + OB_BorderColor * borderOpacity;

    return output;
}