#include "Share.fx"

cbuffer SkyCBuffer : register(b4)
{
    float4 g_Center;
    float4 g_Apex;
    float g_Height;
    float3 g_Empty1231241445;
};

struct VS_OUTPUT_2POS
{
    float4 vPos : SV_POSITION0;
    float3 oPos : POSITION0;
};

VS_OUTPUT_2POS SkyBoxVS(VS_INPUT_POS input)
{
    VS_OUTPUT_2POS output = (VS_OUTPUT_2POS) 0;

    float3 TempPos = input.vPos - (g_Pivot * g_Length);

    output.vPos = mul(float4(TempPos, 1.0f), g_WVP);
    output.oPos = input.vPos;

    return output;
}

PS_OUTPUT_GBUFFER SkyBoxPS(VS_OUTPUT_2POS input)
{
    PS_OUTPUT_GBUFFER output = (PS_OUTPUT_GBUFFER) 0;
    
    float4 Ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 Diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 Specular = float4(0.0f, 0.0f, 0.0f, 0.0f);

    ////정점의 높이 (정점의 높이가 1을 벗어날 수도 있다)
    ////함수는 0보다 낮은값은 0, 1보다 높은값은 1로 고정시키는 함수.
    float y = saturate(input.oPos.y);

    output.vAlbedo = lerp(g_Center, g_Apex, y * g_Height);
    output.vNormal.xyz = float3(1.0f, 1.0f, 1.0f);
    output.vNormal.w = g_Material.Specular.w;
    output.vDepth.rgb = input.vPos.z / input.vPos.w;
    output.vDepth.a = input.vPos.w;
    output.vMaterial.r = float4(1.0f, 1.0f, 1.0f, 1.0f);
    output.vMaterial.g = float4(1.0f, 1.0f, 1.0f, 1.0f);
    output.vMaterial.b = float4(1.0f, 1.0f, 1.0f, 1.0f);
    output.vMaterial.a = float4(1.0f, 1.0f, 1.0f, 1.0f);

    return output;
    //PS_OUTPUT_SINGLE output = (PS_OUTPUT_SINGLE) 0;
    //output.vTarget0 = lerp(g_Center, g_Apex, y * g_Height);

    //return output;


}