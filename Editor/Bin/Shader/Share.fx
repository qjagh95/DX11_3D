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

struct PS_OUTPUT_SINGLE
{
    float4 vTarget0 : SV_Target;
};

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
    matrix g_World;       //월드행렬
    matrix g_View;        //뷰행렬
    matrix g_Projection;  //투영행렬

    //두개를 더 선언하는 이유는 버텍스마다 곱하는 것 보다
    //이미 곱해진 값이 들어와서 연산하는게 훨씬더 빠르기때문이다.
    matrix g_WV;    //World * View
    matrix g_WVP;   //World * View * Projection

    float3 g_Pivot;
    float g_Empty1;
    float3 g_Length;
    float g_Empty2;
}

cbuffer Material : register(b1)
{
    float4 g_MaterialDiffuse;
}

cbuffer Componeent : register(b2)
{
    int g_Animation2DEnable;
    float3 g_Empty;
}

//변수라인
/////////////////////////////////////////////////////////////////////

Texture2D Diffuse : register(t0);
SamplerState DiffuseSampler : register(s0);
Texture2D TargetDiffuse : register(t10);

/////////////////////////////////////////////////////////////////////