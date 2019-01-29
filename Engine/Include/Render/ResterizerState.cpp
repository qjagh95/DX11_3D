#include "stdafx.h"
#include "ResterizerState.h"

JEONG_USING

ResterizerState::ResterizerState()
{
	m_State = RS_RASTERIZER;
}

ResterizerState::~ResterizerState()
{
}

bool ResterizerState::CreateState(D3D11_FILL_MODE eFill, D3D11_CULL_MODE eCull, BOOL bFrontCounterClockwise, int iDepthBias, float fDepthBiasClamp, float fSlopeScaledDepthBias, BOOL bDepthClipEnable, BOOL bScissorEnable, BOOL bMultisampleEnable, BOOL bAntialiasedLineEnable)
{
	D3D11_RASTERIZER_DESC	tDesc;

	tDesc.FillMode = eFill;
	tDesc.CullMode = eCull;
	tDesc.FrontCounterClockwise = bFrontCounterClockwise;
	tDesc.DepthBias = iDepthBias;
	tDesc.DepthBiasClamp = fDepthBiasClamp;
	tDesc.SlopeScaledDepthBias = fSlopeScaledDepthBias;
	tDesc.DepthClipEnable = bDepthClipEnable;
	tDesc.ScissorEnable = bScissorEnable;
	tDesc.MultisampleEnable = bMultisampleEnable;
	tDesc.AntialiasedLineEnable = bAntialiasedLineEnable;

	if (FAILED(Device::Get()->GetDevice()->CreateRasterizerState(&tDesc, (ID3D11RasterizerState**)&m_RenderState)))
		return false;

	return true;
}

void ResterizerState::SetState()
{
	Device::Get()->GetContext()->RSGetState((ID3D11RasterizerState**)&m_OldRenderState);
	Device::Get()->GetContext()->RSSetState((ID3D11RasterizerState*)m_RenderState);
}

void ResterizerState::ResetState()
{
	Device::Get()->GetContext()->RSSetState((ID3D11RasterizerState*)m_OldRenderState);
	SAFE_RELEASE(m_OldRenderState);
}

