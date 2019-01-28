#pragma once
#include "RenderState.h"

JEONG_BEGIN

//이클래스는 2D 3D모드에따라서 모든 오브젝트들에게 하나씩 깊이버퍼를 사용하거나, 사용하지 않을것을 
//디바이스에 알리기위한것이다.
class DepthStancilState : public RenderState
{
public:
	bool CreateState(BOOL DepthEnable, D3D11_DEPTH_WRITE_MASK Mask = D3D11_DEPTH_WRITE_MASK_ALL , D3D11_COMPARISON_FUNC DepthFunc = D3D11_COMPARISON_LESS, BOOL StencilEnable = FALSE, UINT8 StencilReadMask = 0, UINT8 StencilWriteMask = 0, D3D11_DEPTH_STENCILOP_DESC FrontFace = {}, D3D11_DEPTH_STENCILOP_DESC BackFace = {});

	void SetState() override;
	void ResetState() override;

private:
	UINT m_StencilRef;
	UINT m_OldStencilRef;

private:
	DepthStancilState();
	~DepthStancilState();

public:
	friend class RenderManager;
};

JEONG_END