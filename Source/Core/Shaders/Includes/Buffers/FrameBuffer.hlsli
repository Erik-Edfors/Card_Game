
cbuffer FrameBuffer : register(b0)
{
    float4x4 FB_View;
    float4x4 FB_Projection;
    float4 FB_CameraPosition;
}