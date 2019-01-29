#pragma once
#include "RenderState.h"

JEONG_BEGIN

class JEONG_DLL ResterizerState : public RenderState
{
	bool CreateState(D3D11_FILL_MODE eFill = D3D11_FILL_SOLID, D3D11_CULL_MODE eCull = D3D11_CULL_BACK,
		BOOL bFrontCounterClockwise = FALSE, int iDepthBias = 0,
		float fDepthBiasClamp = 0.f, float fSlopeScaledDepthBias = 0.f,
		BOOL bDepthClipEnable = TRUE, BOOL bScissorEnable = FALSE,
		BOOL bMultisampleEnable = FALSE, BOOL bAntialiasedLineEnable = FALSE);

	void SetState() override;
	void ResetState() override;

private:
	UINT m_StencilRef;
	UINT m_OldStencilRef;

private:
	ResterizerState();
	~ResterizerState();

public:
	friend class RenderManager;
};

JEONG_END