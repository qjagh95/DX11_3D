#include "Share.fx"

SamplerState g_GBufferSampler : register(s10);
Texture2D g_GBufferAlbedoTex : register(t10);
Texture2D g_GBufferNormalTex : register(t11);
Texture2D g_GBufferDepthTex : register(t12);
Texture2D g_GBufferMaterialTex : register(t13);

Texture2D g_LightDiffuseTex : register(t14);
Texture2D g_LightSpcularTex : register(t15);

struct PS_OUTPUT_LIGHTACC
{
    float4 vDiffuse : SV_TARGET0;
    float4 vSpecluar : SV_TARGET1;
};

VS_OUTPUT_UV LightAccVS(uint iVertexID : SV_VertexID)
{
    VS_OUTPUT_UV output = (VS_OUTPUT_UV) 0;

    output.vPos = float4(NULLPos[iVertexID], 0.0f, 1.0f);
    output.vUV = NULLUV[iVertexID];

    return output;
}

VS_OUTPUT_UV LightPointVS(VS_INPUT_POS input)
{
    VS_OUTPUT_UV output = (VS_OUTPUT_UV) 0;

    float4 vProjPos = mul(float4(input.vPos, 1.0f), g_WVP);
    output.vPos = vProjPos;

    output.vUV.x = vProjPos.x / vProjPos.w * 0.5f + 0.5f;
    output.vUV.y = -vProjPos.y / vProjPos.w * 0.5f + 0.5f;

    return output;
}

PS_OUTPUT_LIGHTACC LightAccPS(VS_OUTPUT_UV input)
{
    PS_OUTPUT_LIGHTACC output = (PS_OUTPUT_LIGHTACC) 0;

    //SV_POSITION = ���� ������ WVP��ȯ �� ViewPortSize�� �������� ����.
    float2 UV = input.vPos.xy / g_ViewPortSize.xy;

    float4 vDepth = g_GBufferDepthTex.Sample(g_GBufferSampler, UV);
    float4 vNormal = g_GBufferNormalTex.Sample(g_GBufferSampler, UV);
    float4 vMaterial = g_GBufferMaterialTex.Sample(g_GBufferSampler, UV);

    float3 WVPPos;
    //WVP��ȯ�� ���� �Ϸ�� UV�� ������ NDC��ǥ�� ��ȯ���ش�.(-1 ~ 1)
    WVPPos.x = UV.x * 2.0f - 1.0f;
    WVPPos.y = UV.y * -2.0f + 1.0f;
    //Depth�� A���� Projection��ȯ ���� Z���� ����Ǿ��ִ�.
    WVPPos.z = vDepth.r;

    //�����Ͷ��������� W������ ���ڴ�� �������� �ٽ� �����༭ WVP��ȯ�� ��ǥ�� ���Ѵ�.
    float4 Result = float4(WVPPos, 1.0f);
    Result *= vDepth.a;

    //NDC�� ������ HCS(WV��ȯ��ǥ View�� �������� �ϰ�����)�� ���Ѵ�.
    float3 ViewPosition = mul(Result, g_InvProjection).xyz;

    float3 ToCamera = normalize(-ViewPosition);

    float4 Ambient = DecompressColor(vMaterial.r);
    float4 Diffuse = DecompressColor(vMaterial.g);
    float4 Specluar = DecompressColor(vMaterial.b);

    if (g_Light.LightType == LIGHT_DIRECTION)
    {
        if (vDepth.w == 1.0f)
            clip(-1);

        ComputeDirectionLight(vNormal, ToCamera, Ambient, Diffuse, Specluar);
    }
    else if (g_Light.LightType == LIGHT_POINT)
        ComputePointLight(vNormal, ViewPosition, ToCamera, Ambient, Diffuse, Specluar);
    else if (g_Light.LightType == LIGHT_SPOT)
        ComputeSpotLight(vNormal, ViewPosition, ToCamera, Ambient, Diffuse, Specluar);

    output.vDiffuse = Diffuse + Ambient;
    output.vSpecluar = Specluar;

    return output;
}

PS_OUTPUT_SINGLE LightBlendPS(VS_OUTPUT_UV input)
{
    PS_OUTPUT_SINGLE output = (PS_OUTPUT_SINGLE) 0;
    float2 UV = input.vPos.xy / g_ViewPortSize.xy;

    float4 vAlbedo = g_GBufferAlbedoTex.Sample(g_GBufferSampler, UV);

    float4 vDiffuse = g_LightDiffuseTex.Sample(g_GBufferSampler, UV);
    float4 vSpcular = g_LightSpcularTex.Sample(g_GBufferSampler, UV);

    output.vTarget0 = vAlbedo * (vDiffuse + vSpcular);

    return output;
}