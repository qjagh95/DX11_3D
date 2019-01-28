#include "stdafx.h"
#include "DepthStancilState.h"

#include "../Device.h"

JEONG_USING

JEONG::DepthStancilState::DepthStancilState()
{
	m_StencilRef = 0xffffffff;
	m_State = RS_DEPTHSTENCIL;
}

JEONG::DepthStancilState::~DepthStancilState()
{
}

bool JEONG::DepthStancilState::CreateState(BOOL DepthEnable, D3D11_DEPTH_WRITE_MASK Mask, D3D11_COMPARISON_FUNC DepthFunc, BOOL StencilEnable, UINT8 StencilReadMask, UINT8 StencilWriteMask, D3D11_DEPTH_STENCILOP_DESC FrontFace, D3D11_DEPTH_STENCILOP_DESC BackFace)
{
	D3D11_DEPTH_STENCIL_DESC depthDesc;
	depthDesc.DepthEnable = DepthEnable;					//±íÀÌ¹öÆÛ »ç¿ëÇÒ²¨ÀÓ?
	depthDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;	//ALL(¾²±â»ç¿ë), ZERO(²û)
	depthDesc.DepthFunc = DepthFunc;						//¿¬»êÀÚ
	depthDesc.StencilEnable = StencilEnable;				//½ºÅÄ½Ç »ç¿ëÇÒ²¨?
	depthDesc.StencilEnable = StencilEnable;				//½ºÅÄ½Ç »ç¿ëÇÒ²¨?
	depthDesc.StencilReadMask = StencilReadMask; 
	depthDesc.StencilWriteMask = StencilWriteMask;
	depthDesc.FrontFace = FrontFace;
	depthDesc.BackFace = BackFace;

	if (FAILED(Device::Get()->GetDevice()->CreateDepthStencilState(&depthDesc, (ID3D11DepthStencilState**)&m_RenderState)))
		return false;

	return true;
}

void JEONG::DepthStancilState::SetState()
{
	Device::Get()->GetContext()->OMGetDepthStencilState((ID3D11DepthStencilState**)&m_OldRenderState, &m_OldStencilRef);
	Device::Get()->GetContext()->OMSetDepthStencilState((ID3D11DepthStencilState*)m_RenderState, m_StencilRef);
}

void JEONG::DepthStancilState::ResetState()
{
	Device::Get()->GetContext()->OMSetDepthStencilState((ID3D11DepthStencilState*)m_OldRenderState, m_OldStencilRef);
	SAFE_RELEASE(m_OldRenderState);
}