#include "stdafx.h"
#include "MultiRenderTarget.h"
#include "RenderTarget.h"
#include "RenderManager.h"

JEONG_USING
MultiRenderTarget::MultiRenderTarget()
{
	 m_DepthView = NULLPTR;
	 m_OldDepthView = NULLPTR;
}

MultiRenderTarget::~MultiRenderTarget()
{
}

void MultiRenderTarget::AddRenderTargetView(const string& TargetName)
{
	RenderTarget* getTarget = RenderManager::Get()->FindRenderTarget(TargetName);

	if(getTarget == NULLPTR)
		return;

	m_vecTargetView.push_back(getTarget->GetRenderTargetView());
	m_vecOldTargetView.resize(m_vecTargetView.size());
	m_vecShaderResourceView.push_back(getTarget->GetShaderResourceView());
	
	AddDepthView(TargetName);
}

void MultiRenderTarget::AddDepthView(const string & TargetName)
{
	RenderTarget* getTarget = RenderManager::Get()->FindRenderTarget(TargetName);

	if (getTarget == NULLPTR)
		return;

	m_DepthView = getTarget->GetDepthView();
}

void MultiRenderTarget::ReleaseOldTarget()
{
	for (UINT i = 0; i < m_vecOldTargetView.size(); i++)
		SAFE_RELEASE(m_vecOldTargetView[i]);
}

void MultiRenderTarget::ClearRenderTarget(float ClearColor[4])
{
	for (size_t i = 0; i < m_vecTargetView.size(); i++)
		Device::Get()->GetContext()->ClearRenderTargetView(m_vecTargetView[i], ClearColor);
	
	if (m_DepthView != NULLPTR)
		Device::Get()->GetContext()->ClearDepthStencilView(m_DepthView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void MultiRenderTarget::SetTarget()
{
	Device::Get()->GetContext()->OMGetRenderTargets((UINT)m_vecOldTargetView.size(), &m_vecOldTargetView[0], &m_OldDepthView);

	ID3D11DepthStencilView*	pDepth = m_DepthView;

	if (pDepth == NULLPTR)
		pDepth = m_OldDepthView;

	Device::Get()->GetContext()->OMSetRenderTargets((UINT)m_vecTargetView.size(), &m_vecTargetView[0], pDepth);
}

void MultiRenderTarget::ResetTarget()
{
	Device::Get()->GetContext()->OMSetRenderTargets((UINT)m_vecOldTargetView.size(), &m_vecOldTargetView[0], m_OldDepthView);

	ReleaseOldTarget();
	SAFE_RELEASE(m_OldDepthView);
}

void MultiRenderTarget::SetShaderResource(int Register)
{
	Device::Get()->GetContext()->PSSetShaderResources((UINT)Register, (UINT)m_vecShaderResourceView.size(), &m_vecShaderResourceView[0]);
}

void MultiRenderTarget::ResetShaderResource(int Register)
{
	static vector<ID3D11ShaderResourceView*> SRV;
	SRV.resize(m_vecShaderResourceView.size());

	Device::Get()->GetContext()->PSSetShaderResources((UINT)Register, (UINT)m_vecShaderResourceView.size(), &SRV[0]);
}
