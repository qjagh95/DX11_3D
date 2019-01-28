#include "stdafx.h"
#include "Text_Com.h"

//using namespace JEONG;

JEONG::Text_Com::Text_Com()
	: m_TextFormat(NULLPTR), m_TextLayout(NULLPTR), m_Brush(NULLPTR), m_ShadowBrush(NULLPTR),
	m_Size(0.0f), m_Opacity(0.0f), m_ShadowOpacity(0.0f)
{
	m_ComType = CT_TEXT;
	SetTag("Text");

	m_TextType = TRT_UI;
	m_Text = TEXT("Text");
	m_isShadow = true;
	m_isAlpha = false;
	m_isAlphaShadow = false;
	m_ShadowOffset = Vector3(1.0f, 1.0f, 0.0f);

	m_Color = Vector4::Wheat;
	m_ShadowColor = Vector4::Black;
	m_Font = TEXT("±Ã¼­Ã¼");
	m_Size = 20.0f;
	m_Opacity = 1.0f;
	m_ShadowOpacity = 1.0f;

	m_Brush = FontManager::Get()->CreateBrush(m_Color);
	m_ShadowBrush = FontManager::Get()->CreateBrush(m_ShadowColor);
	m_2DTarget = Device::Get()->Get2DRenderTarget();
	m_Align = DWRITE_TEXT_ALIGNMENT_LEADING;
	m_ParagraphAlign = DWRITE_PARAGRAPH_ALIGNMENT_NEAR;
}

JEONG::Text_Com::Text_Com(const Text_Com& CopyData)
	:Component_Base(CopyData)
{
	*this = CopyData;
	m_TextLayout = NULLPTR;
}

JEONG::Text_Com::~Text_Com()
{
	SAFE_RELEASE(m_TextLayout);
}

bool JEONG::Text_Com::Init()
{
	return true;
}

int JEONG::Text_Com::Input(float DeltaTime)
{
	return 0;
}

int JEONG::Text_Com::Update(float DeltaTime)
{
	return 0;
}

int JEONG::Text_Com::LateUpdate(float DeltaTime)
{
	return 0;
}

void JEONG::Text_Com::Collision(float DeltaTime)
{
}

void JEONG::Text_Com::CollisionLateUpdate(float DeltaTime)
{
}

void JEONG::Text_Com::Render(float DeltaTime)
{
	Vector3	vPos = m_Transform->GetWorldPos();

	m_2DTarget->BeginDraw();
	{
		m_TextFormat->SetTextAlignment(m_Align);
		m_TextFormat->SetParagraphAlignment(m_ParagraphAlign);

		if (m_TextLayout != NULLPTR)
		{
			if (m_TextType != TRT_UI)
			{
				Transform_Com* pCameraTr = m_Scene->GetMainCameraTransform();
				vPos -= pCameraTr->GetWorldPos();
			}

			if (m_isShadow == true)
			{
				Vector3	ShadowPos = vPos + m_ShadowOffset;

				if (m_isAlphaShadow == true)
					m_ShadowBrush->SetOpacity(m_ShadowOpacity);
				else
					m_ShadowBrush->SetOpacity(1.0f);

				ShadowPos.y = Device::Get()->GetWinSize().Height - ShadowPos.y;
				m_2DTarget->DrawTextLayout(D2D1::Point2F(ShadowPos.x, ShadowPos.y), m_TextLayout, m_ShadowBrush);
			}

			if (m_isAlpha == true)
				m_Brush->SetOpacity(m_Opacity);
			else
				m_Brush->SetOpacity(1.0f);

			m_2DTarget->DrawTextLayout(D2D1::Point2F(vPos.x, vPos.y), m_TextLayout, m_Brush);
		}
		else 
		{
			D2D1_RECT_F TempArea = m_RenderArea;
			D2D1_RECT_F TempShdowArea = m_RenderArea;

			if (m_TextType != TRT_UI)
			{
				Transform_Com* pCameraTr = m_Scene->GetMainCameraTransform();
				vPos -= pCameraTr->GetWorldPos();
			}

			if (m_isShadow == true)
			{
				Vector3	ShadowPos = vPos + m_ShadowOffset;

				if (m_isAlphaShadow == true)
					m_ShadowBrush->SetOpacity(m_ShadowOpacity);
				else
					m_ShadowBrush->SetOpacity(1.0f);
				
				TempShdowArea.left += ShadowPos.x;
				TempShdowArea.right += ShadowPos.x;
				TempShdowArea.top += Device::Get()->GetWinSize().Height - ShadowPos.y;
				TempShdowArea.bottom += Device::Get()->GetWinSize().Height - ShadowPos.y;

				m_2DTarget->DrawTextW(m_Text.c_str(), (UINT32)m_Text.length(), m_TextFormat, TempShdowArea, m_ShadowBrush);
			}

			if (m_isAlpha == true)
				m_Brush->SetOpacity(m_Opacity);
			else
				m_Brush->SetOpacity(1.0f);

			TempArea.left += vPos.x;
			TempArea.right += vPos.x;
			TempArea.top += Device::Get()->GetWinSize().Height - vPos.y;
			TempArea.bottom += Device::Get()->GetWinSize().Height - vPos.y;

			m_2DTarget->DrawTextW(m_Text.c_str(), (UINT32)m_Text.length(), m_TextFormat, TempArea, m_Brush);
		}
	}
	m_2DTarget->EndDraw();
}

JEONG::Text_Com * JEONG::Text_Com::Clone()
{
	return new Text_Com(*this);
}

void JEONG::Text_Com::AfterClone()
{
}

void JEONG::Text_Com::SetFont(TCHAR * pFont, float fSize)
{
	m_Font = pFont;
	m_Size = fSize;
	m_Color = Vector4::Black;

	m_TextFormat = FontManager::Get()->FontCreate(m_Font, (int)fSize, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STRETCH_ULTRA_EXPANDED,fSize, L"ko");
	m_Brush = FontManager::Get()->CreateBrush(m_Color.x, m_Color.y, m_Color.z, m_Color.w);
}

void JEONG::Text_Com::SetShadowColor(float r, float g, float b, float a)
{
	m_ShadowColor = Vector4(r, g, b, a);
	m_ShadowBrush = FontManager::Get()->CreateBrush(r, g, b, a);
}

void JEONG::Text_Com::SetShadowColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	m_ShadowColor = Vector4(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f);
	m_ShadowBrush = FontManager::Get()->CreateBrush(r, g, b, a);
}

void JEONG::Text_Com::SetShadowColor(const Vector4 & vColor)
{
	m_ShadowColor = vColor;
	m_ShadowBrush = FontManager::Get()->CreateBrush(vColor);
}

void JEONG::Text_Com::SetColor(float r, float g, float b, float a)
{
	m_Color = Vector4(r, g, b, a);
	m_Brush = FontManager::Get()->CreateBrush(r, g, b, a);
}

void JEONG::Text_Com::SetColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	m_Color = Vector4(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f);
	m_Brush = FontManager::Get()->CreateBrush(r, g, b, a);
}

void JEONG::Text_Com::SetColor(const Vector4 & vColor)
{
	m_Color = vColor;
	m_Brush = FontManager::Get()->CreateBrush(vColor);
}