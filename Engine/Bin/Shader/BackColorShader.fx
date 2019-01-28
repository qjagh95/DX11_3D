#include "Share.fx"

VS_OUTPUT_UV BackColorVS(VS_INPUT_UV input)
{
    VS_OUTPUT_UV output = (VS_OUTPUT_UV) 0;

    float3 vPos = input.vPos - g_Pivot * g_Length;

    output.vPos = mul(float4(vPos, 1.0f), g_WVP);

    return output;
}

PS_OUTPUT_SINGLE BackColorPS(VS_OUTPUT_UV input)
{
    PS_OUTPUT_SINGLE output = (PS_OUTPUT_SINGLE) 0;

    output.vTarget0 = g_MaterialDiffuse;

    return output;
}