//쉐이더파일을 중간에 변경이 가능하다.

//예를들어 배경이미지를 그냥 출력하다가 갑자기 피격시 빨간 배경으로 바껴야한다면 빨강쉐이더로 순간적으로 체인지 후 
//다시 기본쉐이더로 돌아오기가 가능함 (9에서도 가능)

//변수 뒤에 : (시맨틱)

//쉐이더는 1줄짜리코드라도 갯수만큼 실행되기때문에 한줄짜리가 아니다
//버텍스가 2만개라면 한줄짜리코드가 2만줄짜리 코드가된다.

struct VS_INPUT_COLOR
{
    //시맨틱은 이름 뒤에 숫자를 붙여준다. 안붙일경우 기본으로 0
    //변수이름 변수명 : 비트필드(비트수)
    float3 vPos : POSITION;
    float4 vColor : COLOR;
};

struct VS_OUTPUT_COLOR
{
    //SV = SystemValue -> 바꿀 수 없다.
    float4 vPos : SV_POSITION;
    float3 vProjPos : POSITION;
    float4 vColor : COLOR;
};

/////////////////////////////////////////////////////////////////////
struct VS_INPUT_POS
{
    float3 vPos : POSITION;
};

struct VS_OUTPUT_POS
{
    float4 vPos : SV_POSITION;
};

/////////////////////////////////////////////////////////////////////
struct VS_INPUT_UV
{
    float3 vPos : POSITION; //뷰포트 Pos
    float2 vUV : TEXCOORD;
};

struct VS_OUTPUT_UV
{
    float4 vPos : SV_POSITION;
    float2 vUV : TEXCOORD;
};

/////////////////////////////////////////////////////////////////////

struct PS_OUTPUT_SINGLE
{
    float4 vTarget0 : SV_Target;
};

/////////////////////////////////////////////////////////////////////

struct VS_INPUT_NORMAL_COLOR
{
    float3 vPos : POSITION;
    float3 vNormal : NORMAL;
    float4 vColor : COLOR;
};

struct VS_OUTPUT_NORMAL_COLOR
{
    float4 vPos : SV_POSITION;
    float3 vPosV : POSITION0;
    float3 vNormalV : NORMAL0; //법선View
    float3 vNormal : NORMAL1; //법선
    float4 vColor : COLOR;
};

struct VS_INPUT_3D
{
    float3 vPos : POSITION;
    float3 vNormal : NORMAL;
    float2 vUV : TEXCOORD;
    float3 vTangent : TANGENT;
    float3 vBinormal : BINORMAL;
    float4 vBlendWeight : BLENDWEIGHTS;
    float4 vBlendIndex : BLENDINDICES;
};

struct VS_OUTPUT_3D
{
    float4 vPos : SV_POSITION;
    float4 vProjPos : POSITION;
    float3 vViewPos : POSITION1;
    float3 vNormal : NORMAL;
    float2 vUV : TEXCOORD;
    float3 vTangent : TANGENT;
    float3 vBinormal : BINORMAL;
    float4 vBlendWeight : BLENDWEIGHTS;
    float4 vBlendIndex : BLENDINDICES;
};

/////////////////////////////////////////////////////////////////////

struct PS_OUTPUT_GBUFFER
{
    float4 vAlbedo : SV_Target0;
    float4 vNormal : SV_Target1;
    float4 vDepth : SV_Target2;
    float4 vMaterial : SV_Target3;
};

/////////////////////////////////////////////////////////////////////

static const float2 NULLPos[4] =
{
    float2(-1.0f, 1.0f),
	float2(1.0f, 1.0f),
	float2(-1.0f, -1.0f),
	float2(1.0f, -1.0f)
};

static const float2 NULLUV[4] =
{
    float2(0.0f, 0.0f),
	float2(1.0f, 0.0f),
	float2(0.0f, 1.0f),
	float2(1.0f, 1.0f)
};

/////////////////////////////////////////////////////////////////////

//CBuffer는 상수버퍼 (C++코드에서 쉐이더로 값을 받아오기 위함)
//cbuffer 함수명 : 레지스터에 b0에 저장하겠다.
cbuffer Transform : register(b0)
{
    matrix g_World; //월드행렬
    matrix g_View; //뷰행렬
    matrix g_Projection; //투영행렬

    //두개를 더 선언하는 이유는 버텍스마다 곱하는 것 보다
    //이미 곱해진 값이 들어와서 연산하는게 훨씬더 빠르기때문이다.
    matrix g_WV; //World * View
    matrix g_WVP; //World * View * Projection
    matrix g_InvProjection;

    float3 g_Pivot;
    float g_Empty1;
    float3 g_Length;
    float g_Empty2;
}

struct MaterialInfo
{
    float4 Diffuse;
    float4 Ambient;
    float4 Specular;
    float4 Emissive;
};

struct LightInfo
{
    float4 LightDiffuse;
    float4 LightAmbient;
    float4 LightSpecular;
    float3 LightDirection;
    int LightType;
    float3 LightPos;
    float LightRange;
    float3 Attenuation; //감쇠
    float LightInAngle;
    float LightOutAngle;
    float FallOff;
    float2 g_Empty123456357;
};

struct SkinningData
{
    float3 vPos;
    float3 vNormal;
    float3 vTangent;
    float3 vBinormal;
};

cbuffer Material : register(b1)
{
    //반사 결정.
    MaterialInfo g_Material;
}

cbuffer Componeent : register(b2)
{
    int g_Animation2DEnable;
    float3 g_Empty;
}

cbuffer Public : register(b10)
{
    float g_DeltaTime;
    float g_PlusedDeltaTime;
    float g_ProjectionFar;
    int g_isDeferred;
    int g_isWireMode;
    float2 g_ViewPortSize;
    float g_Empty123124522534;
}

cbuffer Light : register(b3)
{
    LightInfo g_Light;
}

#define RENDER_FORWARD 0
#define RENDER_DEFERRED 1

//변수라인
/////////////////////////////////////////////////////////////////////

Texture2D DiffuseTexture : register(t0);
Texture2D NormalTexture : register(t1);
Texture2D SpecularTexture : register(t2);

Texture2D BoneTexture : register(t3);

SamplerState DiffuseSampler : register(s0);
Texture2D TargetDiffuse : register(t10);

/////////////////////////////////////////////////////////////////////

#define	LIGHT_DIRECTION	0
#define	LIGHT_POINT	1
#define	LIGHT_SPOT	2
#define	LIGHT_SPOT_BOMI	3
/////////////////////////////////////////////////////////////////////

//out -> 넣어주면 값채워서 반환해줌
void ComputeDirectionLight(float4 vNormal, float3 vToCamera, out float4 Ambient, out float4 Diffuse, out float4 Specular)
{
    float3 LightDir = normalize(g_Light.LightDirection);
    //공식.
    Ambient = g_Material.Ambient * g_Light.LightAmbient;
    //Light를 바라보는 방향, LightDir은 World좌표 기준이라서 View로 변환해준다
    float3 vToLight = mul(float4(LightDir, 1.0f), g_View).xyz;
    vToLight = -normalize(vToLight);

    //cos == 내적값 렘버트 자동적용. 
    Diffuse = g_Material.Diffuse * g_Light.LightDiffuse * max(dot(vToLight, vNormal.xyz), 0.0f);

    //불린 퐁. 빛을 보는 방향과 카메라를 보는 방향을 더한 중간벡터를 사용하겠다. 
    float3 vHalfWay = normalize(vToLight + vToCamera);
    Specular = g_Material.Specular * g_Light.LightSpecular * pow(max(0.0f, dot(vHalfWay, vNormal.xyz)), vNormal.w);
}

void ComputePointLight(float4 vNormal, float3 vPos, float3 vToCamera, out float4 Ambient, out float4 Diffuse, out float4 Specular)
{
    //뷰 공간변환
    float3 LightPos = mul(float4(g_Light.LightPos, 1.0f), g_View).xyz;
    float Distance = distance(LightPos, vPos);

    //이놈때문에 뚫려보임.
    if (g_Light.LightRange < Distance)
    {
        Ambient = float4(0.2f, 0.2f, 0.2f, 1.0f);
        Diffuse = float4(0.2f, 0.2f, 0.2f, 1.0f);
        Specular = float4(0.2f, 0.2f, 0.2f, 1.0f);
        return;
    }

    //라이트의 위치가 존재하기때문.
    float3 vToLight = normalize(LightPos - vPos);
    float3 vHalfWay = normalize(vToLight + vToCamera);
    float LightStrong;

    float4 matAmbient = Ambient;
    float4 matDiffuse = Diffuse;
    float4 matSpecular = Specular;

    //거리에따른 감쇠량
    LightStrong = 1.0f / dot(g_Light.Attenuation, float3(1.0f, Distance, Distance * Distance));

    Ambient = matAmbient * g_Light.LightAmbient;
    Diffuse = matDiffuse * g_Light.LightDiffuse * max(dot(vToLight, vNormal.xyz), 0.0f) * LightStrong;
    Specular = float4(matSpecular.xyz, 1.0f) * g_Light.LightSpecular * pow(max(0.0f, dot(vHalfWay, vNormal.xyz)), matSpecular.w) * LightStrong;
}

void ComputeSpotLight(float4 vNormal, float3 vPos, float3 vToCamera, out float4 Ambient, out float4 Diffuse, out float4 Specular)
{
    float3 LightPos = mul(float4(g_Light.LightPos, 1.0f), g_View).xyz;
    float Distance = distance(LightPos, vPos);

    if (g_Light.LightRange < Distance)
    {
        Ambient = float4(0.2f, 0.2f, 0.2f, 1.0f);
        Diffuse = float4(0.2f, 0.2f, 0.2f, 1.0f);
        Specular = float4(1.2f, 1.2f, 1.2f, 1.0f);
        return;
    }

    float4 matAmbient = Ambient;
    float4 matDiffuse = Diffuse;
    float4 matSpecular = Specular;

    float3 vToLight = normalize(LightPos - vPos);
    float3 vHalfWay = normalize(vToLight + vToCamera);
    float LightStrong;
    float SpotStrong;
    SpotStrong = pow(dot(-vToLight, normalize(g_Light.LightDirection)), g_Light.FallOff);
    LightStrong = 1.0f / dot(g_Light.Attenuation, float3(1.0f, Distance, Distance * Distance));

    float4 a = float4(1.0f, 1.0f, 1.0f, 1.0f);

    Ambient = matAmbient * g_Light.LightAmbient;
    Diffuse = matDiffuse * g_Light.LightDiffuse * max(dot(vToLight, vNormal.xyz), 0.0f) * SpotStrong * LightStrong;
    Specular = float4(matSpecular.xyz, 1.0f) * g_Light.LightSpecular * pow(max(0.0f, dot(vHalfWay, vNormal.xyz)), g_Material.Specular.w) * SpotStrong * LightStrong;
}

void ComputeSpotBomiLight(float4 vNormal, float3 vPos, float3 vToCamera, out float4 Ambient, out float4 Diffuse, out float4 Specular)
{
    float3 LightPos = mul(float4(g_Light.LightPos, 1.0f), g_View).xyz;
    float Distance = distance(LightPos, vPos);

    if (g_Light.LightRange < Distance)
    {
        Ambient = float4(0.2f, 0.2f, 0.2f, 1.0f);
        Diffuse = float4(0.2f, 0.2f, 0.2f, 1.0f);
        Specular = float4(0.2f, 0.2f, 0.2f, 1.0f);
        return;
    }

    float4 matAmbient = Ambient;
    float4 matDiffuse = Diffuse;
    float4 matSpecular = Specular;

    float3 vToLight = normalize(LightPos - vPos);
    float3 vHalfWay = normalize(vToLight + vToCamera);
    float LightStrong;
    float SpotStrong;
    SpotStrong = pow(dot(-vToLight, normalize(g_Light.LightDirection)), g_Light.FallOff);
    LightStrong = 1.0f / dot(g_Light.Attenuation, float3(1.0f, Distance, Distance * Distance));

    Ambient = matAmbient * g_Light.LightAmbient;
    Diffuse = matDiffuse * g_Light.LightDiffuse * max(dot(vToLight, vNormal.xyz), 0.0f) * SpotStrong * LightStrong;
    Specular = float4(matSpecular.xyz, 1.0f) * g_Light.LightSpecular * pow(max(0.0f, dot(vHalfWay, vNormal.xyz)), g_Material.Specular.w) * SpotStrong * LightStrong;
}

//픽셀압축
//Convert Color
float CompressColor(float4 vColor)
{
    uint4 vColor1 = (uint4) 0;
    vColor1.r = uint(vColor.r * 255);
    vColor1.g = uint(vColor.g * 255);
    vColor1.b = uint(vColor.b * 255);
    vColor1.a = uint(vColor.a * 255);

    uint OutColor = 0;
    //바로 넣고
    OutColor = (uint) vColor1.a;
    //8비트 밀고 넣는다
    OutColor = (OutColor << 8) | vColor1.r;
    OutColor = (OutColor << 8) | vColor1.g;
    OutColor = (OutColor << 8) | vColor1.b;

    //float으로 보다 정확하게 형변환해주는 함수.
    return asfloat(OutColor);
}

// 압축 해제
float4 DecompressColor(float Color)
{
    uint inColor = asuint(Color);

    float4 OutColor;
    OutColor.b = (inColor & 0x000000ff) / 255.0f;
    OutColor.g = (inColor >> 8 & 0x000000ff) / 255.0f;
    OutColor.r = (inColor >> 16 & 0x000000ff) / 255.0f;
    OutColor.a = (inColor >> 24 & 0x000000ff) / 255.0f;

    return OutColor;
}

matrix GetBoneMatrix(int index)
{
    matrix matBone =
    {
        BoneTexture.Load(int3(index * 4, 0, 0)),
		BoneTexture.Load(int3(index * 4 + 1, 0, 0)),
		BoneTexture.Load(int3(index * 4 + 2, 0, 0)),
		BoneTexture.Load(int3(index * 4 + 3, 0, 0))
    };

    return matBone;
}

SkinningData Skinned(float3 vPos, float3 vNormal, float3 vTangent, float3 vBinormal, float4 vWeights, float4 vIndices)
{
    SkinningData tSkinning = (SkinningData) 0;

    float fWeights[4];
    fWeights[0] = vWeights.x;
    fWeights[1] = vWeights.y;
    fWeights[2] = vWeights.z;
    fWeights[3] = 1.0f - vWeights.x - vWeights.y - vWeights.z;

    for (int i = 0; i < 4; ++i)
    {
        matrix matBone = GetBoneMatrix((int) vIndices[i]);

        tSkinning.vPos += fWeights[i] * mul(float4(vPos, 1.0f), matBone).xyz;
        tSkinning.vNormal += fWeights[i] * mul(float4(vNormal, 0.0f), matBone).xyz;
        tSkinning.vTangent += fWeights[i] * mul(float4(vTangent, 0.0f), matBone).xyz;
        tSkinning.vBinormal += fWeights[i] * mul(float4(vBinormal, 0.0f), matBone).xyz;
    }

    tSkinning.vNormal = normalize(tSkinning.vNormal);
    tSkinning.vTangent = normalize(tSkinning.vTangent);
    tSkinning.vBinormal = normalize(tSkinning.vBinormal);

    return tSkinning;
}

SkinningData Skinned(float3 vPos, float3 vNormal, float4 vWeights, float4 vIndices)
{
    SkinningData tSkinning = (SkinningData) 0;

    float fWeights[4];
    fWeights[0] = vWeights.x;
    fWeights[1] = vWeights.y;
    fWeights[2] = vWeights.z;
    fWeights[3] = 1.0f - vWeights.x - vWeights.y - vWeights.z;

    for (int i = 0; i < 4; ++i)
    {
        matrix matBone = GetBoneMatrix((int) vIndices[i]);

        tSkinning.vPos += fWeights[i] * mul(float4(vPos, 1.0f), matBone).xyz;
        tSkinning.vNormal += fWeights[i] * mul(float4(vNormal, 0.0f), matBone).xyz;
    }

    tSkinning.vNormal = normalize(tSkinning.vNormal);

    return tSkinning;
}


VS_OUTPUT_3D Vertex3DVS(VS_INPUT_3D input)
{
    VS_OUTPUT_3D output = (VS_OUTPUT_3D)0;

    return output;
}

PS_OUTPUT_SINGLE Vertex3DPS(VS_OUTPUT_3D input)
{
    PS_OUTPUT_SINGLE output = (PS_OUTPUT_SINGLE) 0;

    return output;
}

