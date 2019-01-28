#pragma once

JEONG_BEGIN

class RenderTarget;
class JEONG_DLL MultiRenderTarget
{
public:
	void AddRenderTargetView(const string& TargetName);
	void AddDepthView(const string& TargetName);

	void ClearRenderTarget(float ClearColor[4]);
	void ReleaseOldTarget();

	void SetTarget();
	void ResetTarget();

	void SetShaderResource(int Register = 10);
	void ResetShaderResource(int Register = 10);

private:
	vector<ID3D11RenderTargetView*> m_vecTargetView;
	vector<ID3D11ShaderResourceView*> m_vecShaderResourceView;
	vector<ID3D11RenderTargetView*> m_vecOldTargetView;
	ID3D11DepthStencilView* m_DepthView;
	ID3D11DepthStencilView* m_OldDepthView;

public:
	~MultiRenderTarget();

private:
	MultiRenderTarget();

public:
	friend class RenderManager;
};

JEONG_END