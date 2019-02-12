#pragma once
JEONG_BEGIN

struct RenderGroup
{
	class GameObject** ObjectList;
	int	Size;
	int	Capacity;

	RenderGroup()
	{
		Size = 0;
		Capacity = 100;
		ObjectList = new GameObject*[Capacity];
	}

	~RenderGroup()
	{
		SAFE_DELETE_ARRARY(ObjectList);
	}
};

class RenderState;
class BlendState;
class RenderTarget;
class GameObject;
class Light_Com;
class MultiRenderTarget;
class Mesh;
class JEONG_DLL RenderManager
{
public:
	bool Init();

	void AddBlendTargetDesc(BOOL bEnable, D3D11_BLEND srcBlend = D3D11_BLEND_SRC_ALPHA, D3D11_BLEND destBlend = D3D11_BLEND_INV_SRC_ALPHA, D3D11_BLEND_OP blendOp = D3D11_BLEND_OP_ADD, D3D11_BLEND srcAlphaBlend = D3D11_BLEND_ONE, D3D11_BLEND destAlphaBlend = D3D11_BLEND_ZERO, D3D11_BLEND_OP blendAlphaOp = D3D11_BLEND_OP_ADD, UINT8 iWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL);
	bool CreateDepthStencilState(const string& KeyName, BOOL bDepthEnable, D3D11_DEPTH_WRITE_MASK eMask = D3D11_DEPTH_WRITE_MASK_ZERO, D3D11_COMPARISON_FUNC eDepthFunc = D3D11_COMPARISON_LESS, BOOL bStencilEnable = FALSE, UINT8 iStencilReadMask = 0, UINT8 iStencilWriteMask = 0, D3D11_DEPTH_STENCILOP_DESC tFrontFace = {}, D3D11_DEPTH_STENCILOP_DESC tBackFace = {});
	bool CreateRenderTarget(const string& KeyName, DXGI_FORMAT TargetFormat, const Vector3& Pos, const Vector3& Scale, bool isDebugDraw = true, const Vector4& ClearColor = Vector4(0.0f, 0.0f, 0.0f, 1.0f), DXGI_FORMAT DepthFormat = DXGI_FORMAT_UNKNOWN);
	bool CreateBlendState(const string& KeyName, BOOL bAlphaCoverage = FALSE, BOOL bIndependent = FALSE);
	bool CreateResterizerState(const string& KeyName, D3D11_FILL_MODE eFill = D3D11_FILL_SOLID, D3D11_CULL_MODE eCull = D3D11_CULL_BACK, BOOL bFrontCounterClockwise = FALSE, int iDepthBias = 0, float fDepthBiasClamp = 0.0f, float fSlopeScaledDepthBias = 0.0f, BOOL bDepthClipEnable = TRUE, BOOL bScissorEnable = FALSE, BOOL bMultisampleEnable = FALSE, BOOL bAntialiasedLineEnable = FALSE);

	void SetGameMode(GAME_MODE eMode) { m_GameMode = eMode; }
	GAME_MODE GetGameMode() const { return m_GameMode; }

	RenderState* FindRenderState(const string& KeyName);
	RenderState* FindRenderStateNoneCount(const string& KeyName);
	RenderTarget* FindRenderTarget(const string& KeyName);

	void EnableDeferredRender() { m_isDeferred = true; }
	bool GetIsRenderMode() const { return m_isDeferred; }
	void AddRenderObject(GameObject* object);
	void Render(float DeltaTime);

	bool CreateMultiTarget(const string& MultiKey);
	bool AddMultiRenderTarget(const string& MultiKey, const string& TargetKey);
	bool AddMultiRenderTargetDepthView(const string& MultiKey, const string& TargetKey);

	MultiRenderTarget* FindMultiTarget(const string& MultiKey);

private:
	void Render3D(float DeltaTime);
	void RenderGBuffer(float DeltaTime);

	void ForwardRender(float DeltaTime);
	void DeferredRender(float DeltaTime);
	void RenderLightAcc(float DeltaTime);

	void RenderDirectionLight(float DeltaTime, Light_Com* light);
	void RenderPointLight(float DeltaTime, Light_Com* light);
	void RenderSpotLight(float DeltaTime, Light_Com* light);
	void RenderFullScreen(float DeltaTime);
	void RenderLightBlend(float DeltaTime);
	void SetIsWireFrame(bool Value) { m_isWireFrame = Value; }

private:
	GAME_MODE m_GameMode;

	unordered_map<string, RenderState*> m_RenderStateMap;
	unordered_map<string, RenderTarget*> m_RenderTargetMap;
	unordered_map<string, MultiRenderTarget*> m_MultiTargetMap;

	BlendState* m_CreateState;
	RenderGroup m_RenderGroup[RG_END];
	RenderGroup m_LightGroup;
	Sampler* m_GBufferSampler;
	Shader* m_LightAccDirShader;
	Shader* m_LightAccPointShader;
	Shader* m_LightAccSpotShader;
	Shader* m_LightAccBlendShader;
	Shader* m_FullScreenShader;

	MultiRenderTarget* m_GBufferMultiTarget;
	MultiRenderTarget* m_LightMultiTarget;
	RenderTarget* m_LightBlendTarget;
	RenderTarget* m_LightDiffuseTarget;
	RenderTarget* m_LightSpcularTarget;

	RenderTarget* m_AlbedoTarget;
	RenderState* m_AddBlend;
	RenderState* m_DepthDisable;
	RenderState* m_DepthGrator;
	RenderState* m_DepthLess;
	RenderState* m_FrontCull;
	RenderState* m_BackCull;
	RenderState* m_WireFrame;
	RenderState* m_CullNone;
	RenderState* m_ZeroBlend;
	RenderState* m_AllBlend;

	bool m_isDeferred;
	bool m_isWireFrame;
	PublicCBuffer m_CBuffer;

	Mesh* m_SphereVolum;
	Mesh* m_CornVolum;

public:
	CLASS_IN_SINGLE(RenderManager)
};

JEONG_END