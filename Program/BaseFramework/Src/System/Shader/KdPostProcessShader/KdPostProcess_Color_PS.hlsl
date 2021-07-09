#include "inc_KdPostProcess.hlsli"

cbuffer cb : register(b0)
{
    float4 g_color; //
};

Texture2D g_inputTex : register(t0);
SamplerState g_ss : register(s0);

float4 main(VSOutput In) : SV_Target0
{
    float4 texColor = g_inputTex.Sample(g_ss, In.UV);
    return texColor * g_color;
}
