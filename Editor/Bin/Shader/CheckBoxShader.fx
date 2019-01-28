#include "Share.fx"

cbuffer CheckBox : register(b9)
{
    int isCheck;
    float3 Empty;
    float4 CheckBoxColor;
}

VS_OUTPUT_UV CheckBoxVS(VS_INPUT_UV input)
{
    VS_OUTPUT_UV output = (VS_OUTPUT_UV) 0;

    float3 vPos = input.vPos - g_Pivot * g_Length;

    output.vPos = mul(float4(vPos, 1.0f), g_WVP);
    output.vUV = input.vUV;

    return output;
}

PS_OUTPUT_SINGLE CheckBoxPS(VS_OUTPUT_UV input)
{
    PS_OUTPUT_SINGLE output = (PS_OUTPUT_SINGLE) 0;

    output.vTarget0 = Diffuse.Sample(DiffuseSampler, input.vUV);
    
    if (isCheck == 0)
    {
        if (output.vTarget0.a > 0.5f)
            output.vTarget0 = float4(1.0f, 1.0f, 1.0f, 1.0f);
    }

    //xÇ¥½Ã
    //if (input.vUV.x == input.vUV.y)
    //    output.vTarget0 = float4(1.0f, 0.0f, 0.0f, 1.0f);
    //if (input.vUV.y == 1.0f - input.vUV.x)
    //    output.vTarget0 = float4(1.0f, 0.0f, 0.0f, 1.0f);

    return output;
}