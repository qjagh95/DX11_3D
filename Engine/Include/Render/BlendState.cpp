#include "stdafx.h"
#include "BlendState.h"

#include "../Device.h"

JEONG_USING

JEONG::BlendState::BlendState()
{
	m_SamplerMask = 0xffffffff;
	m_State = RS_BLEND;
}

JEONG::BlendState::~BlendState()
{
}

//BlendDesc설정
bool JEONG::BlendState::CreateState(BOOL bAlphaCoverage, BOOL bIndependent)
{
	D3D11_BLEND_DESC tDesc = {};

	tDesc.AlphaToCoverageEnable = bAlphaCoverage;
	tDesc.IndependentBlendEnable = bIndependent;

	//DESC안에있는 RenderTarget(8개)에 전부 같은값으로 초기화한다.
	for (size_t i = 0; i < m_vecTargetDesc.size(); ++i)
		memcpy(&tDesc.RenderTarget[i], &m_vecTargetDesc[i], sizeof(D3D11_RENDER_TARGET_BLEND_DESC));

	m_vecTargetDesc.clear();

	if (FAILED(Device::Get()->GetDevice()->CreateBlendState(&tDesc, (ID3D11BlendState**)&m_RenderState)))
	{
		TrueAssert(true);
		return false;
	}
	return true;
}

void JEONG::BlendState::AddTargetDesc(BOOL bEnable, D3D11_BLEND srcBlend, D3D11_BLEND destBlend, D3D11_BLEND_OP blendOp, D3D11_BLEND srcAlphaBlend, D3D11_BLEND destAlphaBlend, D3D11_BLEND_OP blendAlphaOp, UINT8 iWriteMask)
{
	D3D11_RENDER_TARGET_BLEND_DESC tDesc = {};
	tDesc.BlendEnable = bEnable;
	tDesc.SrcBlend = srcBlend;
	tDesc.DestBlend = destBlend;
	tDesc.BlendOp = blendOp;
	tDesc.SrcBlendAlpha = srcAlphaBlend;
	tDesc.DestBlendAlpha = destAlphaBlend;
	tDesc.BlendOpAlpha = blendAlphaOp;
	tDesc.RenderTargetWriteMask = iWriteMask;

	m_vecTargetDesc.push_back(tDesc);
}

void JEONG::BlendState::SetBlendFactor(float Factor[4])
{
	memcpy(m_BlendFactor, Factor, sizeof(float) * 4);
}

void JEONG::BlendState::SetState()
{
	// Get함수를 이용해서 state를 얻어오게 되면 레퍼런스 카운트가 증가한다.
	Device::Get()->GetContext()->OMGetBlendState((ID3D11BlendState**)&m_OldRenderState, m_OldBlendFactor, &m_OldSamplerMask);
	Device::Get()->GetContext()->OMSetBlendState((ID3D11BlendState*)m_RenderState, m_BlendFactor, m_SamplerMask);
}

void JEONG::BlendState::ResetState()
{
	Device::Get()->GetContext()->OMSetBlendState((ID3D11BlendState*)m_OldRenderState, m_OldBlendFactor, m_OldSamplerMask);
	SAFE_RELEASE(m_OldRenderState);
}


