#include "stdafx.h"
#include "FontManager.h"
#include "Device.h"

JEONG_USING
SINGLETON_VAR_INIT(FontManager)

FontManager::FontManager()
	:m_WriteFactory(NULLPTR)
{
}

FontManager::~FontManager()
{
	SAFE_RELEASE(m_WriteFactory);
	Safe_Release_Map(m_BrushMap);
	Safe_Release_Map(m_FontMap);
}

bool FontManager::Init()
{
	if (FAILED(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(m_WriteFactory), (IUnknown**)&m_WriteFactory)))
		return false;

	FontCreate(TEXT("궁서체"), DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_ULTRA_EXPANDED, 20.0f, TEXT("ko"));

	CreateBrush(0.0f, 0.0f, 0.0f, 1.0f);
	CreateBrush(1.0f, 0.0f, 0.0f, 1.0f);
	CreateBrush(0.0f, 1.0f, 0.0f, 1.0f);
	CreateBrush(0.0f, 0.0f, 1.0f, 1.0f);
	CreateBrush(1.0f, 0.8431f, 0.0f, 1.0f);

	return true;
}

IDWriteTextFormat * FontManager::FontCreate(const TCHAR* FontName, int Weight, int Style, int Stretch, float Size, const TCHAR * LocalName)
{
	char cKey[MAX_PATH] = {};

#ifdef UNICODE
	WideCharToMultiByte(CP_ACP, 0, FontName, -1, cKey, lstrlen(FontName) * 2, 0, 0);
#else
	strcpy_s(strKey, pFontName);
#endif // UNICODE
	sprintf_s(cKey, "%s%d", cKey, (int)Size);

	IDWriteTextFormat* newFormat = FindFont(cKey);

	if (newFormat != NULLPTR)
		return newFormat;

	// Text Format을 설정한다.
	// 1번인자 : 폰트 이름을 입력한다. 
	// 2번인자 : 특정 폰트는 컬렉션을 가지고 있다. 예 Arial 은 Arial Black 이런식
	// 3번인자 : 폰트 굵기 4번인자 : 기울기
	// 5번인자 : 자간 6번인자 : 폰트 크기
	// 7번인자 : 언어 지역 이름을 설정한다. 한국은 ko - KR 미국은 en - us 
	// 8번인자 : 텍스트 인터페이스
	if (FAILED(m_WriteFactory->CreateTextFormat(FontName, NULLPTR, (DWRITE_FONT_WEIGHT)Weight,(DWRITE_FONT_STYLE)Style,	(DWRITE_FONT_STRETCH)Stretch, Size, LocalName, &newFormat)))
		return NULLPTR;

	m_FontMap.insert(make_pair(cKey, newFormat));

	return newFormat;
}

ID2D1SolidColorBrush * FontManager::CreateBrush(float r, float g, float b, float a)
{
	ID2D1SolidColorBrush* newBrush = FindBrush(r, g, b, a);

	if (newBrush != NULLPTR)
		return newBrush;

	if (FAILED(Device::Get()->Get2DRenderTarget()->CreateSolidColorBrush(D2D1::ColorF(r, g, b, a), &newBrush)))
		return NULLPTR;

	unsigned int iKey = CreateColorKey(r, g, b, a);

	m_BrushMap.insert(make_pair(iKey, newBrush));

	return newBrush;
}

ID2D1SolidColorBrush * FontManager::CreateBrush(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	ID2D1SolidColorBrush* newBrush = FindBrush(r, g, b, a);

	if (newBrush != NULLPTR)
		return newBrush;

	if (FAILED(Device::Get()->Get2DRenderTarget()->CreateSolidColorBrush(D2D1::ColorF(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f), &newBrush)))
		return NULLPTR;

	unsigned int iKey = CreateColorKey(r, g, b, a);

	m_BrushMap.insert(make_pair(iKey, newBrush));

	return newBrush;
}

ID2D1SolidColorBrush * FontManager::CreateBrush(const Vector4 & vColor)
{
	float	r, g, b, a;
	r = vColor.r;
	g = vColor.g;
	b = vColor.b;
	a = vColor.a;

	ID2D1SolidColorBrush* newBrush = FindBrush(r, g, b, a);

	if (newBrush != NULLPTR)
		return newBrush;

	if (FAILED(Device::Get()->Get2DRenderTarget()->CreateSolidColorBrush(D2D1::ColorF(r, g, b, a), &newBrush)))
		return NULLPTR;

	unsigned int iKey = CreateColorKey(r, g, b, a);

	m_BrushMap.insert(make_pair(iKey, newBrush));

	return newBrush;
}

IDWriteTextLayout * FontManager::CreateTextLayout(const TCHAR * Text, IDWriteTextFormat * Format, float Width, float Height)
{
	IDWriteTextLayout* pLayout = NULLPTR;

	if (FAILED(m_WriteFactory->CreateTextLayout(Text, lstrlen(Text), Format, Width, Height, &pLayout)))
		return NULLPTR;

	return pLayout;
}

IDWriteTextLayout * FontManager::CreateTextLayout(const TCHAR * Text, const string & FontKey, float Width, float Height)
{
	IDWriteTextLayout* pLayout = NULLPTR;
	IDWriteTextFormat* pFormat = FindFont(FontKey);

	if (FAILED(m_WriteFactory->CreateTextLayout(Text, lstrlen(Text), pFormat, Width, Height, &pLayout)))
		return NULLPTR;

	return pLayout;

}

unsigned int FontManager::CreateColorKey(float r, float g, float b, float a)
{
	return CreateColorKey((unsigned char)(r * 255.0f), (unsigned char)(g * 255.0f), (unsigned char)(b * 255.0f), (unsigned char)(a * 255.0f));
}

unsigned int FontManager::CreateColorKey(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	//비트밀고 저장함.
	unsigned int iKey = a;

	iKey <<= 8;
	iKey |= r;
	iKey <<= 8;
	iKey |= g;
	iKey <<= 8;
	iKey |= b;

	return iKey;
}

IDWriteTextFormat * FontManager::FindFont(const string & KeyName)
{
	unordered_map<string, IDWriteTextFormat*>::iterator FindIter = m_FontMap.find(KeyName);

	if (FindIter == m_FontMap.end())
		return NULLPTR;

	return FindIter->second;
}

ID2D1SolidColorBrush * FontManager::FindBrush(float r, float g, float b, float a)
{
	return FindBrush((unsigned char)(r * 255.0f ), (unsigned char)(g * 255.0f), (unsigned char)(b * 255.0f), (unsigned char)(a * 255.0f));
}

ID2D1SolidColorBrush * FontManager::FindBrush(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	unsigned int iKey = CreateColorKey(r, g, b, a);

	unordered_map<unsigned int, ID2D1SolidColorBrush*>::iterator FindIter = m_BrushMap.find(iKey);

	if (FindIter == m_BrushMap.end())
		return nullptr;

	return FindIter->second;
}

ID2D1SolidColorBrush * FontManager::FindBrush(const Vector4 & vColor)
{
	unsigned int	iKey = CreateColorKey(vColor.r, vColor.g, vColor.b,	vColor.a);

	unordered_map<unsigned int, ID2D1SolidColorBrush*>::iterator FindIter = m_BrushMap.find(iKey);

	if (FindIter == m_BrushMap.end())
		return nullptr;

	return FindIter->second;
}


