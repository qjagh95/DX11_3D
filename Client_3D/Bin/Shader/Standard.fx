#include "Share.fx"
//fx파일은 버텍스와 픽셀쉐이더 두개 동시에 처리가 가능하다.

cbuffer Animation2D : register(b8)
{
    float2 g_LeftTopUV;
    float2 g_RightBottomUV;
    int g_Frame;
    float3 g_Empty3;
}

//////////////////////////////////ColorShader//////////////////////////

VS_OUTPUT_COLOR Standard_Color_VS(VS_INPUT_COLOR input)
{
    //0으로 초기화
    VS_OUTPUT_COLOR output = (VS_OUTPUT_COLOR)0;
    //mul = 곱하기 (벡터행렬곱)
    output.vPos = mul(float4(input.vPos, 1.0f), g_WVP);
    output.vColor = input.vColor;

    return output;
}

//버텍스 쉐이더에서 뱉은 OutPut데이터가 픽셀쉐이더 Input으로 들어온다. (매쉬클래스 랜더에서 이미 다 지정을 해놨다(IA~) )
PS_OUTPUT_SINGLE Standard_Color_PS(VS_OUTPUT_COLOR input)
{
    PS_OUTPUT_SINGLE output = (PS_OUTPUT_SINGLE)0;

    output.vTarget0 = g_MaterialDiffuse;

    return output;
}

//////////////////////////////////ColorShader//////////////////////////


//////////////////////////////////UVShader/////////////////////////////

VS_OUTPUT_UV Standard_UV_VS(VS_INPUT_UV input)
{
    VS_OUTPUT_UV output = (VS_OUTPUT_UV)0;

    //API때 했던 중심점공식 (newPos = Pos - Size * Pivot)이거랑 똑같음.
    //버텍스의 투영공간변환 전 좌표가 0.n상태에서 중심점을 잡는다.
    float3 TempPos = input.vPos - (g_Pivot * g_Length);

    output.vPos = mul(float4(TempPos, 1.0f), g_WVP);

    //애니메이션이 있다면 UV조절 후 출력 (UV값은 CPU연산 후 들어온다)
    //없다면 그냥 출력
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
  
PS_OUTPUT_SINGLE Standard_UV_PS(VS_OUTPUT_UV input)
{
    PS_OUTPUT_SINGLE output = (PS_OUTPUT_SINGLE)0;
    
    //Diffuse(Texture2D)에 SampleState(재질정보와 UV)를 넣어주고 색상정보를 곱한다
    output.vTarget0 = Diffuse.Sample(DiffuseSampler, input.vUV) * g_MaterialDiffuse;

    return output;
}
//////////////////////////////////UVShader/////////////////////////////

//////////////////////////////////STAITC Shader/////////////////////////////

VS_OUTPUT_UV StandardTexStaticVS(VS_INPUT_UV input)
{
    VS_OUTPUT_UV output = (VS_OUTPUT_UV) 0;

    float3 TempPos = input.vPos - g_Pivot * g_Length;

    output.vPos = mul(float4(TempPos, 1.0f), g_WVP);
    output.vUV = input.vUV;

    return output;
}

PS_OUTPUT_SINGLE StandardTexStaticPS(VS_OUTPUT_UV input)
{
    PS_OUTPUT_SINGLE output = (PS_OUTPUT_SINGLE) 0;

    output.vTarget0 = Diffuse.Sample(DiffuseSampler, input.vUV);

    return output;
}

//////////////////////////////////NULLBuffer출력용/////////////////////////////

VS_OUTPUT_UV FullScreenVS(uint VertexID : SV_VertexID)
{
    VS_OUTPUT_UV output = (VS_OUTPUT_UV) 0;

    output.vPos = float4(NULLPos[VertexID], 0.0f, 1.0f);
    output.vUV = NULLUV[VertexID];

    return output;
}

PS_OUTPUT_SINGLE FullScreenPS(VS_OUTPUT_UV input)
{
    PS_OUTPUT_SINGLE output = (PS_OUTPUT_SINGLE) 0;

    output.vTarget0 = Diffuse.Sample(DiffuseSampler, input.vUV);
    return output;
}