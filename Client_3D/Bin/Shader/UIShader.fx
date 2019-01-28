#include "Share.fx"

cbuffer Button : register(b9)
{
    float4 g_ButtonColor;
}

cbuffer Animation2D : register(b8)
{
    float2 g_LeftTopUV;
    float2 g_RightBottomUV;
    int g_Frame;
    float3 g_Empty3;
}

VS_OUTPUT_UV UIVS(VS_INPUT_UV input)
{
    VS_OUTPUT_UV output = (VS_OUTPUT_UV) 0;

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

PS_OUTPUT_SINGLE UIPS(VS_OUTPUT_UV input)
{
    PS_OUTPUT_SINGLE output = (PS_OUTPUT_SINGLE) 0;

    output.vTarget0 = Diffuse.Sample(DiffuseSampler, input.vUV) * g_ButtonColor;   

    return output;
}