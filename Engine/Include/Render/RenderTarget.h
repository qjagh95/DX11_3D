
JEONG_BEGIN

class Shader;
class Mesh;
class DepthStancilState;
class Sampler;
class JEONG_DLL RenderTarget
{
public:
	bool CreateRenderTarget(DXGI_FORMAT TargetFormat, const Vector3& Pos, const Vector3& Scale, DXGI_FORMAT DepthFormat = DXGI_FORMAT_UNKNOWN);
	void SetDrawDebug(bool isDraw);
	void ClearTarget();
	void SetTarget();
	void ResetTarget();
	void Render(float DeltaTime);
	void RenderFullScreen();
	void SetShader(int Register);
	void SetClearColor(float ClearColor[4]) { memcpy(m_ClearColor, ClearColor, sizeof(float) * 4); }
	void SetClearColor(const Vector4& Color) { memcpy(m_ClearColor, &Color, sizeof(Vector4)); }

private:
	ID3D11Texture2D* m_TargetBuffer;
	ID3D11RenderTargetView*	m_TargetView;
	ID3D11ShaderResourceView* m_TargetShaderResourceView;

	ID3D11Texture2D* m_DepthBuffer;
	ID3D11DepthStencilView*	m_DepthView;

	ID3D11RenderTargetView*	m_OldTargetView;
	ID3D11DepthStencilView* m_OldDepthView;
	float m_ClearColor[4];
	bool m_isDebugDraw;

	Shader*	m_Shader;
	Shader* m_FullScreenShader;
	Mesh* m_Mesh;
	Sampler* m_Sampler;
	ID3D11InputLayout* m_Layout;
	DepthStancilState* m_DepthState;
	Vector3	m_Pos;
	Vector3	m_Scale;

public:
	RenderTarget();
	~RenderTarget();
};

JEONG_END