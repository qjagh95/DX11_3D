//���̴������� �߰��� ������ �����ϴ�.

//������� ����̹����� �׳� ����ϴٰ� ���ڱ� �ǰݽ� ���� ������� �ٲ����Ѵٸ� �������̴��� ���������� ü���� �� 
//�ٽ� �⺻���̴��� ���ƿ��Ⱑ ������ (9������ ����)

//���� �ڿ� : (�ø�ƽ)

//���̴��� 1��¥���ڵ�� ������ŭ ����Ǳ⶧���� ����¥���� �ƴϴ�
//���ؽ��� 2������� ����¥���ڵ尡 2����¥�� �ڵ尡�ȴ�.

struct VS_INPUT_COLOR
{
    //�ø�ƽ�� �̸� �ڿ� ���ڸ� �ٿ��ش�. �Ⱥ��ϰ�� �⺻���� 0
    //�����̸� ������ : ��Ʈ�ʵ�(��Ʈ��)
    float3 vPos : POSITION;
    float4 vColor : COLOR;
};

struct VS_OUTPUT_COLOR
{
    //SV = SystemValue -> �ٲ� �� ����.
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
    float3 vPos : POSITION; //����Ʈ Pos
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
    float3 vPosV : POSITION;
    float3 vNormalV : NORMAL0; //����View
    float3 vNormal : NORMAL1; //����
    float4 vColor : COLOR;
};

/////////////////////////////////////////////////////////////////////

struct PS_OUTPUT_GBUFFER
{
    float4 vAlbedo : SV_Target;
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

//CBuffer�� ������� (C++�ڵ忡�� ���̴��� ���� �޾ƿ��� ����)
//cbuffer �Լ��� : �������Ϳ� b0�� �����ϰڴ�.
cbuffer Transform : register(b0)
{
    matrix g_World;       //�������
    matrix g_View;        //�����
    matrix g_Projection;  //�������

    //�ΰ��� �� �����ϴ� ������ ���ؽ����� ���ϴ� �� ����
    //�̹� ������ ���� ���ͼ� �����ϴ°� �ξ��� �����⶧���̴�.
    matrix g_WV;    //World * View
    matrix g_WVP;   //World * View * Projection

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
    float4 Emission;
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
    float3 Attenuation; //����
    float LightInAngle;
    float LightOutAngle;
    float FallOff;
    float2 g_Empty123456357;
};

cbuffer Material : register(b1)
{
    //�ݻ� ����.
    MaterialInfo g_Material;
}

cbuffer Componeent : register(b2)
{
    int g_Animation2DEnable;
    int g_isDeferred;
    float2 g_Empty;
}

cbuffer Public : register(b10)
{
    float g_DeltaTime;
    float g_PlusedDeltaTime;
    float2 g_Empty12345;
}

cbuffer Light : register(b3)
{
    LightInfo g_Light;
}

#define RENDER_FORWARD 0
#define RENDER_DEFERRED 1

//��������
/////////////////////////////////////////////////////////////////////

Texture2D Diffuse : register(t0);
SamplerState DiffuseSampler : register(s0);
Texture2D TargetDiffuse : register(t10);

/////////////////////////////////////////////////////////////////////

#define	LIGHT_DIRECTION	0
#define	LIGHT_POINT	1
#define	LIGHT_SPOT	2
#define	LIGHT_SPOT_BOMI	3
/////////////////////////////////////////////////////////////////////

//out -> �־��ָ� ��ä���� ��ȯ����
void ComputeDirectionLight(float3 vNormal, float3 vToCamera, out float4 Ambient, out float4 Diffuse, out float4 Specular)
{
    //����.
    Ambient = g_Material.Ambient * g_Light.LightAmbient;

    //Light�� �ٶ󺸴� ����, LightDir�� World��ǥ �����̶� View�� ��ȯ���ش�
    float3 vToLight = normalize(mul(float4(-g_Light.LightDirection, 0.0f), g_View).xyz);

    //cos == ������ ����Ʈ �ڵ�����. 
    Diffuse = g_Material.Diffuse * g_Light.LightDiffuse * max(dot(vToLight, vNormal), 0.0f);

    //�Ҹ� ��. ���� ���� ����� ī�޶� ���� ������ ���� �߰����͸� ����ϰڴ�. 
    float3 vHalfWay = normalize(vToLight + vToCamera);
    Specular = g_Material.Specular * g_Light.LightSpecular * max(dot(vHalfWay, vNormal), 0.0f);
}

void ComputePointLight(float3 vNormal, float3 vPos, float3 vToCamera, out float4 Ambient, out float4 Diffuse, out float4 Specular)
{
    //�� ������ȯ
    float3 LightPos = mul(float4(g_Light.LightPos, 1.0f), g_View).xyz;
    float Distance = distance(LightPos, vPos);

    if(g_Light.LightRange < Distance)
    {
        Ambient = g_Material.Ambient * g_Light.LightAmbient * 0.1f;
        Diffuse = g_Material.Diffuse * g_Light.LightDiffuse * 0.1f;
        Specular = g_Material.Specular * g_Light.LightSpecular * 0.1f;
        return;
    }

    //����Ʈ�� ��ġ�� �����ϱ⶧��.
    float3 vToLight = normalize(LightPos - vPos);
    float3 vHalfWay = normalize(vToLight + vToCamera);
    float LightStrong;

    //�Ÿ������� ���跮
    LightStrong = 1.0f / dot(g_Light.Attenuation, float3(1.0f, Distance, Distance * Distance));

    Ambient = g_Material.Ambient * g_Light.LightAmbient * LightStrong;
    Diffuse = g_Material.Diffuse * g_Light.LightDiffuse * max(dot(vToLight, vNormal), 0.0f) * LightStrong;
    Specular = g_Material.Specular * g_Light.LightSpecular * max(dot(vHalfWay, vNormal), 0.0f) * LightStrong;
}

void ComputeSpotLight(float3 vNormal, float3 vPos, float3 vToCamera, out float4 Ambient, out float4 Diffuse, out float4 Specular)
{
    float3 LightPos = mul(float4(g_Light.LightPos, 1.0f), g_View).xyz;
    float Distance = distance(LightPos, vPos);

    if (g_Light.LightRange < Distance)
    {
        Ambient = g_Material.Ambient * g_Light.LightAmbient * 0.1f;
        Diffuse = g_Material.Diffuse * g_Light.LightDiffuse * 0.1f;
        Specular = g_Material.Specular * g_Light.LightSpecular * 0.1f;
        return;
    }

    float3 vToLight = normalize(LightPos - vPos);
    float3 vHalfWay = normalize(vToLight + vToCamera);
    float SpotStrong;
    SpotStrong = pow(max(dot(-vToLight, g_Light.LightDirection), 0.0f), g_Light.FallOff);

    Ambient = g_Material.Ambient * g_Light.LightAmbient * SpotStrong;
    Diffuse = g_Material.Diffuse * g_Light.LightDiffuse * max(dot(vToLight, vNormal), 0.0f) * SpotStrong;
    Specular = g_Material.Specular * g_Light.LightSpecular * max(dot(vHalfWay, vNormal), 0.0f) * SpotStrong;
}

void ComputeSpotBomiLight(float3 vNormal, float3 vPos, float3 vToCamera, out float4 Ambient, out float4 Diffuse, out float4 Specular)
{
    float3 LightPos = mul(float4(g_Light.LightPos, 1.0f), g_View).xyz;
    float Distance = distance(LightPos, vPos);

    if (g_Light.LightRange < Distance)
    {
        Ambient = g_Material.Ambient * g_Light.LightAmbient * 0.1f;
        Diffuse = g_Material.Diffuse * g_Light.LightDiffuse * 0.1f;
        Specular = g_Material.Specular * g_Light.LightSpecular * 0.1f;
        return;
    }

    float3 vToLight = normalize(LightPos - vPos);
    float3 vHalfWay = normalize(vToLight + vToCamera);
    float LightStrong;
    float SpotStrong;
    SpotStrong = pow(max(dot(-vToLight, g_Light.LightDirection), 0.0f), g_Light.FallOff);
    LightStrong = 1.0f / dot(g_Light.Attenuation, float3(1.0f, Distance, Distance * Distance));

    Ambient = g_Material.Ambient * g_Light.LightAmbient * SpotStrong * LightStrong;
    Diffuse = g_Material.Diffuse * g_Light.LightDiffuse * max(dot(vToLight, vNormal), 0.0f) * SpotStrong * LightStrong;
    Specular = g_Material.Specular * g_Light.LightSpecular * max(dot(vHalfWay, vNormal), 0.0f) * SpotStrong * LightStrong;
}

float ConvertColor(float4 vColor)
{
    uint4 vColor1 = (uint4) 0;
    vColor1.r = uint(vColor.r * 255);
    vColor1.g = uint(vColor.g * 255);
    vColor1.b = uint(vColor.b * 255);
    vColor1.a = uint(vColor.a * 255);

    uint InColor = 0;
    //�ٷ� �ְ�
    InColor = (uint) (vColor1.a * 255);
    //8��Ʈ �а� �ִ´�
    InColor = (InColor << 8) | vColor1.r;
    InColor = (InColor << 8) | vColor1.g;
    InColor = (InColor << 8) | vColor1.b;

    //float���� ���� ��Ȯ�ϰ� ����ȯ���ִ� �Լ�.
    return asfloat(InColor);
}

float4 ConvertColor(float Color)
{
    float4 vColor;
    uint inColor = asuint(Color);

    vColor.b = (inColor & 0x000000ff) / 255.0f;
    vColor.g = (inColor >> 8 & 0x000000ff) / 255.0f;
    vColor.r = (inColor >> 16 & 0x000000ff) / 255.0f;
    vColor.a = (inColor >> 24 & 0x000000ff) / 255.0f;

    return vColor;
}