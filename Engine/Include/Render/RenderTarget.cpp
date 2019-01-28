#include "stdafx.h"
#include "RenderTarget.h"
#include "Shader.h"
#include "DepthStancilState.h"

#include "../Resource/Sampler.h"

JEONG_USING

RenderTarget::RenderTarget()
{
	m_TargetBuffer = NULLPTR;
	m_TargetShaderResourceView = NULLPTR;
	m_TargetView = NULLPTR;
	m_DepthBuffer = NULLPTR;
	m_DepthView = NULLPTR;
	m_OldTargetView = NULLPTR;
	m_OldDepthView = NULLPTR;
	m_Shader = NULLPTR;
	m_Mesh = NULLPTR;
	m_Layout = NULLPTR;
	m_DepthState = NULLPTR;
	m_FullScreenShader = NULLPTR;
	m_Sampler = NULLPTR;

	ZeroMemory(m_ClearColor, sizeof(float) * 4);
}

RenderTarget::~RenderTarget()
{
	SAFE_RELEASE(m_TargetBuffer);
	SAFE_RELEASE(m_TargetShaderResourceView);
	SAFE_RELEASE(m_TargetView);
	SAFE_RELEASE(m_DepthBuffer);
	SAFE_RELEASE(m_DepthView);
	SAFE_RELEASE(m_OldTargetView);
	SAFE_RELEASE(m_OldDepthView);
	SAFE_RELEASE(m_Shader);
	SAFE_RELEASE(m_Mesh);
	SAFE_RELEASE(m_DepthState);
	SAFE_RELEASE(m_FullScreenShader);
	SAFE_RELEASE(m_Sampler);
}

bool RenderTarget::CreateRenderTarget(DXGI_FORMAT TargetFormat, const Vector3 & Pos, const Vector3 & Scale, DXGI_FORMAT DepthFormat)
{
	m_Pos = Pos;
	m_Scale = Scale;

	Vector2 WinSize;
	WinSize.x = (float)Device::Get()->GetWinSize().Width;
	WinSize.y = (float)Device::Get()->GetWinSize().Height;

	// 타겟용 텍스처 생성
	D3D11_TEXTURE2D_DESC textureDesc = {};
	textureDesc.Width = (UINT)WinSize.x;
	textureDesc.Height = (UINT)WinSize.y;
	textureDesc.ArraySize = 1;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	textureDesc.Format = TargetFormat;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;

	if (FAILED(Device::Get()->GetDevice()->CreateTexture2D(&textureDesc, NULLPTR, &m_TargetBuffer)))
		return false;

	// 만들어진 텍스처로 ShaderResourceView와 TargetView를 생성한다.
	if (FAILED(Device::Get()->GetDevice()->CreateShaderResourceView(m_TargetBuffer, NULLPTR, &m_TargetShaderResourceView)))
		return false;

	if (FAILED(Device::Get()->GetDevice()->CreateRenderTargetView(m_TargetBuffer, NULLPTR, &m_TargetView)))
		return false;

	if (DepthFormat != DXGI_FORMAT_UNKNOWN)
	{
		textureDesc.Width = (UINT)WinSize.x;
		textureDesc.Height =(UINT)WinSize.y;
		textureDesc.ArraySize = 1;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		textureDesc.Format = DepthFormat;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;

		if (FAILED(Device::Get()->GetDevice()->CreateTexture2D(&textureDesc, nullptr, &m_DepthBuffer)))
			return false;

		if (FAILED(Device::Get()->GetDevice()->CreateDepthStencilView(m_DepthBuffer, nullptr, &m_DepthView)))
			return false;
	}

	m_Sampler = ResourceManager::Get()->FindSampler(LINER_SAMPLER);

	return true;
}

void RenderTarget::ClearTarget()
{
	Device::Get()->GetContext()->ClearRenderTargetView(m_TargetView, m_ClearColor);

	if (m_DepthView != NULLPTR)
		Device::Get()->GetContext()->ClearDepthStencilView(m_DepthView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void RenderTarget::SetTarget()
{
	// 기존 디바이스에 지정되어 있는 타겟뷰와 깊이뷰를 얻어온다.
	Device::Get()->GetContext()->OMGetRenderTargets(1, &m_OldTargetView, &m_OldDepthView);

	// 만약 깊이버퍼가 생성 안되어 있을 경우 기존에 지정되어 있던
	// 깊이 버퍼를 그대로 지정한다.
	ID3D11DepthStencilView*	pDepth = m_OldDepthView;

	if (m_DepthView != NULLPTR)
		pDepth = m_DepthView;

	Device::Get()->GetContext()->OMSetRenderTargets(1, &m_TargetView, pDepth);
}

void RenderTarget::ResetTarget()
{
	// 원래 타겟들로 교체한다.
	Device::Get()->GetContext()->OMSetRenderTargets(1, &m_OldTargetView, m_OldDepthView);

	SAFE_RELEASE(m_OldDepthView);
	SAFE_RELEASE(m_OldTargetView);
}

void RenderTarget::Render(float DeltaTime)
{
	if (m_isDebugDraw == false)
		return;

	TransformCBuffer tTransform = {};
	Matrix	matPos, matScale;
	matScale.Scaling(m_Scale);
	matPos.Translation(m_Pos);

	Scene* pScene = SceneManager::Get()->GetCurScene();
	Camera_Com*	pCamera = pScene->GetUICamera();

	SAFE_RELEASE(pScene);

	Matrix	matView, matProj;
	matView = pCamera->GetViewMatrix();
	matProj = pCamera->GetProjection();

	tTransform.World = matScale * matPos;
	tTransform.View = matView;
	tTransform.Projection = matProj;
	tTransform.WV = tTransform.World * matView;
	tTransform.WVP = tTransform.WV * matProj;
	tTransform.Lenth = m_Mesh->GetLenth();

	tTransform.World.Transpose();
	tTransform.View.Transpose();
	tTransform.Projection.Transpose();
	tTransform.WV.Transpose();
	tTransform.WVP.Transpose();

	ShaderManager::Get()->UpdateCBuffer("Transform", &tTransform);

	m_DepthState->SetState();
	{
		if (m_Sampler != NULLPTR)
			m_Sampler->SetSamplerState(0);

		Device::Get()->GetContext()->IASetInputLayout(m_Layout);
		Device::Get()->GetContext()->PSSetShaderResources(0, 1, &m_TargetShaderResourceView);

		m_Shader->SetShader();
		m_Mesh->Render();

		ID3D11ShaderResourceView* pSRV = NULLPTR;
		Device::Get()->GetContext()->PSSetShaderResources(0, 1, &pSRV);
	}
	m_DepthState->ResetState();

}

void RenderTarget::RenderFullScreen()
{
	if (m_FullScreenShader == NULLPTR)
		m_FullScreenShader = ShaderManager::Get()->FindShader(FULLSCREEN_SHADER);

	m_DepthState->SetState();
	{			
		if (m_Sampler != NULLPTR)
			m_Sampler->SetSamplerState(0);

		Device::Get()->GetContext()->PSSetShaderResources(0, 1, &m_TargetShaderResourceView);
		m_FullScreenShader->SetShader();

		//NULL Buffer를 이용한 출력준비.
		//NULL Buffer로 전체 화면크기의 사각형을 출력한다.
		Device::Get()->GetContext()->IASetInputLayout(NULLPTR);

		UINT Offset = 0;
		Device::Get()->GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		Device::Get()->GetContext()->IASetVertexBuffers(0, 0, NULLPTR, 0, &Offset);
		Device::Get()->GetContext()->IASetIndexBuffer(0, DXGI_FORMAT_UNKNOWN, 0);
		Device::Get()->GetContext()->Draw(4, 0);

		//NULLBUFFER 사용 후 반드시 NULL로 초기화 해줘야한다.
		ID3D11ShaderResourceView* pSRV = NULLPTR;
		Device::Get()->GetContext()->PSSetShaderResources(0, 1, &pSRV);
	}
	m_DepthState->ResetState();
}

void RenderTarget::SetShader(int Register)
{
	Device::Get()->GetContext()->PSSetShaderResources(Register, 1, &m_TargetShaderResourceView);
}

void RenderTarget::SetDrawDebug(bool isDraw)
{
	m_isDebugDraw = isDraw;

	if (m_isDebugDraw == true)
	{
		m_DepthState = (DepthStancilState*)RenderManager::Get()->FindRenderState(DEPTH_DISABLE);
		m_Mesh = ResourceManager::Get()->FindMesh("TextureRect");
		m_Shader = ShaderManager::Get()->FindShader(STANDARD_UV_STATIC_SHADER);
		m_Layout = ShaderManager::Get()->FindInputLayOut(POS_UV_LAYOUT);
	}
}