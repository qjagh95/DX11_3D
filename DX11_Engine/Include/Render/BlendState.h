#pragma once
#include "RenderState.h"

JEONG_BEGIN

class JEONG_DLL BlendState : public RenderState
{
public:
	void SetState() override;
	void ResetState() override;

	bool CreateState(BOOL bAlphaCoverage, BOOL bIndependent);
	void SetSamplerMask(UINT mask) { m_SamplerMask = mask; }
	void SetBlendFactor(float Factor[4]);
	void AddTargetDesc(BOOL bEnable, D3D11_BLEND srcBlend = D3D11_BLEND_SRC_ALPHA, D3D11_BLEND destBlend = D3D11_BLEND_INV_SRC_ALPHA, D3D11_BLEND_OP blendOp = D3D11_BLEND_OP_ADD, D3D11_BLEND srcAlphaBlend = D3D11_BLEND_ONE, D3D11_BLEND destAlphaBlend = D3D11_BLEND_ZERO, D3D11_BLEND_OP blendAlphaOp = D3D11_BLEND_OP_ADD, UINT8 iWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL);
		
private:
	vector<D3D11_RENDER_TARGET_BLEND_DESC> m_vecTargetDesc;
	float m_BlendFactor[4];
	UINT m_SamplerMask;
	float m_OldBlendFactor[4];
	UINT m_OldSamplerMask;

private:
	BlendState();
	~BlendState();
	
public:
	friend class RenderManager;
};
JEONG_END

