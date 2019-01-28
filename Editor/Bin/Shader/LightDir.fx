#include "Share.fx"

SamplerState g_GBufferSampler : register(s10);
Texture2D g_GBufferAlbedoTex : register(t10);
Texture2D g_GBufferNormalTex : register(t11);
Texture2D g_GBufferDepthTex : register(t12);
Texture2D g_GBufferMaterialTex : register(t13);

VS_OUTPUT_UV LightAccVS(uint iVertexID : SV_VertexID)
{
    VS_OUTPUT_UV output = (VS_OUTPUT_UV) 0;

    output.vPos = float4(NULLPos[iVertexID], 0.f, 1.f);
    output.vUV = NULLUV[iVertexID];

    return output;
}

PS_OUTPUT_SINGLE LightAccPS(VS_OUTPUT_UV input)
{
    PS_OUTPUT_SINGLE output = (PS_OUTPUT_SINGLE) 0;

    output.vTarget0 = g_GBufferNormalTex.Sample(g_GBufferSampler, input.vUV);

    return output;
}