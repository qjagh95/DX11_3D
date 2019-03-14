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
    float4 vNormal : NORMAL;
};

VS_OUTPUT_2POS SkyBoxVS(VS_INPUT_NORMAL_COLOR input)
{
    VS_OUTPUT_2POS output = (VS_OUTPUT_2POS) 0;

    float3 TempPos = input.vPos - (g_Pivot * g_Length);

    output.vPos = mul(float4(TempPos, 1.0f), g_WVP);
    output.oPos = input.vPos;
    output.vNormal = mul(float4(input.vNormal, 0.0f), g_WV);

    return output;
}

PS_OUTPUT_GBUFFER SkyBoxPS(VS_OUTPUT_2POS input)
{
    PS_OUTPUT_GBUFFER output = (PS_OUTPUT_GBUFFER) 0;
   
    //정점의 높이 (정점의 높이가 1을 벗어날 수도 있다)
    //함수는 0보다 낮은값은 0, 1보다 높은값은 1로 고정시키는 함수.
    float y = saturate(input.oPos.y);

    output.vAlbedo = lerp(g_Center, g_Apex, y * g_Height);
    output.vNormal = input.vNormal;
    output.vNormal.w = g_Material.Specular.w;

    output.vMaterial.r = CompressColor(g_Material.Ambient);
    output.vMaterial.g = CompressColor(g_Material.Diffuse);
    output.vMaterial.b = CompressColor(g_Material.Specular);
    output.vMaterial.a = CompressColor(g_Material.Emissive);

    return output;
}