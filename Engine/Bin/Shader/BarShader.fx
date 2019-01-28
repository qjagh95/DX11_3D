#include "Share.fx"

cbuffer Bar : register(b9)
{
    float g_BarPercent;
    int g_BarMoveDir;
    float2 g_BarEmpty;
    float4 g_BarLight;
}

cbuffer Animation2D : register(b8)
{
    float2 g_LeftTopUV;
    float2 g_RightBottomUV;
    int g_Frame;
    float3 g_Empty3;
}

Texture2D LightDiffuse : register(t1);

VS_OUTPUT_UV BarVS(VS_INPUT_UV input)
{
    VS_OUTPUT_UV output = (VS_OUTPUT_UV)0;
    float3 vPos = input.vPos - g_Pivot * g_Length;

    output.vPos = mul(float4(vPos, 1.0f), g_WVP);

    if (g_Animation2DEnable == 1)
    {
        if (input.vUV.x == 0.0f)
            output.vUV.x = g_LeftTopUV.x;
        else
            output.vUV.x = g_RightBottomUV.x;

        if (input.vUV.y == 0.0f)
            output.vUV.y = g_LeftTopUV.y;
        else
            output.vUV.y = g_RightBottomUV.y;
    }
    else
        output.vUV = input.vUV;

    return output;
}

PS_OUTPUT_SINGLE BarPS(VS_OUTPUT_UV input)
{
    PS_OUTPUT_SINGLE output = (PS_OUTPUT_SINGLE) 0;

    float4 OriginColor = Diffuse.Sample(DiffuseSampler, input.vUV);
    float4 LightColor = LightDiffuse.Sample(DiffuseSampler, input.vUV);

    output.vTarget0 = OriginColor * g_BarLight * LightColor;

    return output;
}