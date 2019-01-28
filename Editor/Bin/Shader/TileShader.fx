#include "Share.fx"

cbuffer Collider : register(b8)
{
    float4 g_Color;
}

VS_OUTPUT_POS TileVS(VS_INPUT_POS input)
{
    VS_OUTPUT_POS output = (VS_OUTPUT_POS) 0;

    float3 TempPos = input.vPos - (g_Pivot * g_Length);

    output.vPos = mul(float4(TempPos, 1.0f), g_WVP);

    return output;
}

PS_OUTPUT_SINGLE TilePS()
{
    PS_OUTPUT_SINGLE output = (PS_OUTPUT_SINGLE) 0;

    output.vTarget0 = g_Color;

    return output;
}