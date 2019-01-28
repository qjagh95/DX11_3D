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

    output.vTarget0 = g_Material.Diffuse;

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
    output.vTarget0 = Diffuse.Sample(DiffuseSampler, input.vUV) * g_Material.Diffuse;

    return output;
}
//////////////////////////////////UVShader/////////////////////////////

//////////////////////////////////NormalColor/////////////////////////////
VS_OUTPUT_NORMAL_COLOR StandardNormalColorVS(VS_INPUT_NORMAL_COLOR input)
{
    VS_OUTPUT_NORMAL_COLOR output = (VS_OUTPUT_NORMAL_COLOR) 0;

    float3 vPos = input.vPos - g_Pivot * g_Length;

    output.vPos = mul(float4(input.vPos, 1.0f), g_WVP);
    //뷰공간변환만 된놈을 사용하겠다. (카메라를 원점으로 땡겼기때문에 카메라위치는 0, 0)
    output.vPosV = mul(float4(input.vPos, 1.0f), g_WV).xyz;
    output.vNormalV = normalize(mul(float4(input.vNormal, 0.0f), g_WV).xyz);
    output.vNormal = input.vNormal;
    output.vColor = input.vColor;

    return output;
}

PS_OUTPUT_GBUFFER StandardNormalColorPS(VS_OUTPUT_NORMAL_COLOR input)
{ 
    PS_OUTPUT_GBUFFER output = (PS_OUTPUT_GBUFFER) 0;
    
    float4 Ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 Diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 Specular = float4(0.0f, 0.0f, 0.0f, 0.0f);

    //카메라를 바라보는 방향
    float3 toCamera = -normalize(input.vPosV);

    if (g_isDeferred == RENDER_FORWARD)
    {
        if (g_Light.LightType == LIGHT_DIRECTION)
            ComputeDirectionLight(input.vNormalV, toCamera, Ambient, Diffuse, Specular);
        else if (g_Light.LightType == LIGHT_POINT)
            ComputePointLight(input.vNormalV, input.vPosV, toCamera, Ambient, Diffuse, Specular);
        else if (g_Light.LightType == LIGHT_SPOT)
            ComputeSpotLight(input.vNormalV, input.vPosV, toCamera, Ambient, Diffuse, Specular);
        else if (g_Light.LightType == LIGHT_SPOT_BOMI)
            ComputeSpotBomiLight(input.vNormalV, input.vPosV, toCamera, Ambient, Diffuse, Specular);

        output.vAlbedo = input.vColor * (Ambient + Diffuse) + Specular;
    }
    else
    {
        //조명연산 또는 각종 연산을 전부 하게되면 연산이 너무 많이지기때문에 각각 나눠서 Texture의 rgb값에 저장해놓는다.
        //이게 디퍼드를 사용하는 이유.
        output.vAlbedo = input.vColor;
        output.vNormal.xyz = input.vNormalV;
        output.vNormal.w = g_Light.LightSpecular.w;

        //TODO : 가까운놈 흰색, 먼놈 - 검은색
        output.vDepth.rgb = float3(g_ProjectionFar - input.vPosV.z, g_ProjectionFar - input.vPosV.z, g_ProjectionFar - input.vPosV.z) / g_ProjectionFar;
        output.vDepth.a = input.vPos.w;
        output.vMaterial.r = CompressColor(g_Material.Diffuse);
        output.vMaterial.g = CompressColor(g_Material.Ambient);
        output.vMaterial.b = CompressColor(g_Material.Specular);
        output.vMaterial.a = CompressColor(g_Material.Emissive);
    }

    return output;
}
//////////////////////////////////NormalColor/////////////////////////////


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

