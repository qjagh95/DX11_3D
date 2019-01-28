#pragma once
#include "Component_Base.h"

JEONG_BEGIN

enum TEXT_RENDER_TYPE
{
	TRT_UI,
	TRT_2D,
	TRT_3D
};

class JEONG_DLL Text_Com : public Component_Base
{
public:
	bool Init() override;
	int Input(float DeltaTime) override;
	int Update(float DeltaTime) override;
	int LateUpdate(float DeltaTime) override;
	void Collision(float DeltaTime) override;
	void CollisionLateUpdate(float DeltaTime) override;
	void Render(float DeltaTime) override;
	Text_Com* Clone() override;
	void AfterClone() override;

	void SetTextType(TEXT_RENDER_TYPE eType) { m_TextType = eType; }
	void SetIsShadow(bool bShadow) { m_isShadow = bShadow; }
	void SetIsAlphaBlend(bool bAlpha) { m_isAlpha = bAlpha; }
	void SetIsShadowAlphaBlend(bool bAlpha) { m_isAlphaShadow = bAlpha; }
	void SetShadowOffset(const Vector3& vOffset) { m_ShadowOffset = vOffset; }
	void SetOpacity(float fOpacity) { m_Opacity = fOpacity; }
	void SetShadowOpacity(float fOpacity) { m_ShadowOpacity = fOpacity; }
	void SetSize(float fSize) { m_Size = fSize; }
	void SetText(TCHAR* pText) { m_Text = pText; }
	void SetRenderArea(float Left, float Top, float Right, float Bottom) { m_RenderArea = D2D1::RectF(Left, Top, Right, Bottom); /*CreateLayout();*/ }
	void SetTextAlignMent(DWRITE_TEXT_ALIGNMENT Align = DWRITE_TEXT_ALIGNMENT_LEADING) { m_Align = Align; }
	void SetTextParaGraph(DWRITE_PARAGRAPH_ALIGNMENT Align = DWRITE_PARAGRAPH_ALIGNMENT_NEAR) { m_ParagraphAlign = Align; }

	void SetFont(TCHAR* pFont, float fSize);
	void SetShadowColor(float r, float g, float b, float a);
	void SetShadowColor(unsigned char r, unsigned char g,unsigned char b, unsigned char a);
	void SetShadowColor(const Vector4& vColor);
	void SetColor(float r, float g, float b, float a);
	void SetColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
	void SetColor(const Vector4& vColor);

private:
	IDWriteTextFormat* m_TextFormat;
	IDWriteTextLayout* m_TextLayout;
	ID2D1SolidColorBrush* m_Brush;
	ID2D1SolidColorBrush* m_ShadowBrush;
	ID2D1RenderTarget* m_2DTarget;
	bool m_isShadow;
	wstring m_Text;
	TCHAR* m_Font;
	float m_Size;
	DWRITE_TEXT_ALIGNMENT m_Align;
	DWRITE_PARAGRAPH_ALIGNMENT m_ParagraphAlign;
	bool m_isAlpha;
	bool m_isAlphaShadow;
	float m_Opacity;
	float m_ShadowOpacity;
	Vector4 m_Color;
	Vector4 m_ShadowColor;
	Vector3 m_ShadowOffset;
	D2D1_RECT_F m_RenderArea;
	TEXT_RENDER_TYPE m_TextType;

protected:
	Text_Com();
	Text_Com(const Text_Com& CopyData);
	~Text_Com();

public:
	friend class GameObject;
};

JEONG_END

