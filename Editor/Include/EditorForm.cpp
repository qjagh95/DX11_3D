// EditorForm.cpp : 구현 파일입니다.
//
#include "EditorHeader.h"
#include "Editor.h"
#include "EditorForm.h"
#include "afxwin.h"
#include <BineryReader.h>
#include <BineryWriter.h>

//로그를 텍스트파일로 시간입력해서. (엑셀)
//Control Z 되돌리기..?

// EditorForm

IMPLEMENT_DYNCREATE(EditorForm, CFormView)

EditorForm::EditorForm()
	: CFormView(IDD_DIALOG_EDIT)
	, m_TileCountX(0)
	, m_TileCountY(0)
	, m_TileSizeX(0)
	, m_TileSizeY(0)
	, m_TagName(_T(""))
	, m_ScaleX(0)
	, m_ScaleY(0)
	, m_ScaleZ(0)
	, m_RotationX(0)
	, m_RotationY(0)
	, m_RotationZ(0)
	, m_PosX(0)
	, m_PosY(0)
	, m_PosZ(0)
	, m_StartPosX(0)
	, m_StartPosY(0)
	, m_BackColorR(0)
	, m_BackColorG(0)
	, m_BackColorB(0)
	, m_BackColorA(0)
	, m_TextCount(0)
	, m_isLine(TRUE)
	, m_isImageLoad(false)
	, m_ImageName(_T(""))
	, m_CreateTileCount(0)
{
	m_TileCountX = 0;
	m_TileCountY = 0;
	m_TileSizeX = 0;
	m_TileSizeY = 0;

	m_StageObject = NULLPTR;

	m_StageTransform = NULLPTR;

	m_Path = PathManager::Get()->FindPath(TEXTURE_PATH);
}

EditorForm::~EditorForm()
{
	SAFE_RELEASE(m_StageObject);

	SAFE_RELEASE(m_StageTransform);
}

void EditorForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_TILESELECT, m_TileTypeBox);
	DDX_Control(pDX, IDC_TILEOPTIONSELECT, m_TileOptionBox);
	DDX_Control(pDX, IDC_TILEIMAGESELECT, m_TileImageBox);

	DDX_Text(pDX, IDC_SCALEX2, m_TileCountX);
	DDX_Text(pDX, IDC_SCALEX3, m_TileCountY);
	DDX_Text(pDX, IDC_SCALEX4, m_TileSizeX);
	DDX_Text(pDX, IDC_SCALEX5, m_TileSizeY);
	DDX_Text(pDX, IDC_TAGNAME, m_TagName);
	DDX_Text(pDX, IDC_SCALEX, m_ScaleX);
	DDX_Text(pDX, IDC_SCALEY, m_ScaleY);
	DDX_Text(pDX, IDC_SCALEZ, m_ScaleZ);
	DDX_Text(pDX, IDC_ROTATIONX, m_RotationX);
	DDX_Text(pDX, IDC_ROTATIONY, m_RotationY);
	DDX_Text(pDX, IDC_ROTATIONZ, m_RotationZ);
	DDX_Text(pDX, IDC_POSITIONX, m_PosX);
	DDX_Text(pDX, IDC_POSITIONY, m_PosY);
	DDX_Text(pDX, IDC_POSITIONZ, m_PosZ);
	DDX_Control(pDX, IDC_WORK, m_WorkList);
	DDX_Text(pDX, IDC_STARTPOSX, m_StartPosX);
	DDX_Text(pDX, IDC_STARTPOSY, m_StartPosY);
	DDX_Control(pDX, IDC_STARTPOSX, m_StartPosXControl);
	DDX_Control(pDX, IDC_STARTPOSY, m_StartPosYControl);
	DDX_Text(pDX, IDC_COLORR, m_BackColorR);
	DDX_Text(pDX, IDC_COLORG, m_BackColorG);
	DDX_Text(pDX, IDC_COLORB, m_BackColorB);
	DDX_Text(pDX, IDC_COLORA, m_BackColorA);
	DDX_Check(pDX, IDC_LINEON, m_isLine);
	DDX_CBString(pDX, IDC_TILEIMAGESELECT, m_ImageName);
	DDX_Text(pDX, IDC_CREATETILECOUNT, m_CreateTileCount);

	if (m_TileTypeBox.GetCurSel() == 0)
	{
		GetDlgItem(IDC_STARTPOSX)->EnableWindow(true);
		GetDlgItem(IDC_STARTPOSY)->EnableWindow(true);
	}
	else if (m_TileTypeBox.GetCurSel() == 1)
	{
		GetDlgItem(IDC_STARTPOSX)->EnableWindow(false);
		GetDlgItem(IDC_STARTPOSY)->EnableWindow(false);
	}
	else
	{
		GetDlgItem(IDC_STARTPOSX)->EnableWindow(true);
		GetDlgItem(IDC_STARTPOSY)->EnableWindow(true);
	}


}

BEGIN_MESSAGE_MAP(EditorForm, CFormView)
	ON_EN_CHANGE(IDC_SCALEX, &EditorForm::OnEnChangeScalex)
	ON_EN_CHANGE(IDC_ROTATIONX, &EditorForm::OnEnChangeRotationx)
	ON_EN_CHANGE(IDC_POSITIONX, &EditorForm::OnEnChangePositionx)
	ON_EN_CHANGE(IDC_SCALEY, &EditorForm::OnEnChangeScaley)
	ON_EN_CHANGE(IDC_ROTATIONY, &EditorForm::OnEnChangeRotationy)
	ON_EN_CHANGE(IDC_POSITIONY, &EditorForm::OnEnChangePositiony)
	ON_EN_CHANGE(IDC_SCALEZ, &EditorForm::OnEnChangeScalez)
	ON_EN_CHANGE(IDC_ROTATIONZ, &EditorForm::OnEnChangeRotationz)
	ON_EN_CHANGE(IDC_POSITIONZ, &EditorForm::OnEnChangePositionz)
	ON_CBN_SELCHANGE(IDC_TILESELECT, &EditorForm::OnCbnSelchangeTileselect)
	ON_CBN_SELCHANGE(IDC_TILEOPTIONSELECT, &EditorForm::OnCbnSelchangeTileoptionselect)
	ON_CBN_SELCHANGE(IDC_TILEIMAGESELECT, &EditorForm::OnCbnSelchangeTileimageselect)
	ON_BN_CLICKED(IDC_TILECREATEBUTTON, &EditorForm::OnBnClickedTilecreatebutton)
	ON_BN_CLICKED(IDC_COLORSAVE, &EditorForm::OnBnClickedColorsave)
	ON_EN_CHANGE(IDC_COLORR, &EditorForm::OnEnChangeColorr)
	ON_EN_CHANGE(IDC_COLORG, &EditorForm::OnEnChangeColorg)
	ON_EN_CHANGE(IDC_COLORB, &EditorForm::OnEnChangeColorb)
	ON_EN_CHANGE(IDC_COLORA, &EditorForm::OnEnChangeColora)
	ON_EN_CHANGE(IDC_TILECOUNTX, &EditorForm::OnEnChangeTilecountx)
	ON_EN_CHANGE(IDC_TILECOUNTY, &EditorForm::OnEnChangeTilecounty)
	ON_EN_CHANGE(IDC_TILESIZEX, &EditorForm::OnEnChangeTilesizex)
	ON_EN_CHANGE(IDC_TILESIZEY, &EditorForm::OnEnChangeTilesizey)
	ON_EN_CHANGE(IDC_TAGNAME, &EditorForm::OnEnChangeTagname)
	ON_BN_CLICKED(IDC_TILECLEAR, &EditorForm::OnBnClickedTileclear)
	ON_BN_CLICKED(IDC_LINEON, &EditorForm::OnBnClickedLineon)
	ON_BN_CLICKED(IDC_TILELOAD, &EditorForm::OnBnClickedTileload)
	ON_BN_CLICKED(IDC_TILESAVE, &EditorForm::OnBnClickedTilesave)
	ON_WM_PAINT()
	ON_EN_CHANGE(IDC_CREATETILECOUNT, &EditorForm::OnEnChangeCreatetilecount)
END_MESSAGE_MAP()

// EditorForm 진단입니다.

#ifdef _DEBUG
void EditorForm::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void EditorForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


void EditorForm::AddWorkText(const wstring& Text, int Index)
{
	CTime Time = CTime::GetTickCount();

	wstring Temp2;
	
	Temp2 += to_wstring(Time.GetYear());
	Temp2 += L"년 ";
	Temp2 += to_wstring(Time.GetMonth());
	Temp2 += L"월 ";
	Temp2 += to_wstring(Time.GetDay());
	Temp2 += L"일 ";
	Temp2 += to_wstring(Time.GetHour());
	Temp2 += L"시 ";
	Temp2 += to_wstring(Time.GetMinute());
	Temp2 += L"분 ";
	Temp2 += to_wstring(Time.GetSecond());
	Temp2 += L"초 ";

	m_WorkList.InsertString(Index, Text.c_str());
	ExcelManager::Get()->WriteData("EditorLog", 0, m_TextCount, Text);
	ExcelManager::Get()->WriteData("EditorLog", 6, m_TextCount, Temp2);

	m_TextCount++;
}

void EditorForm::AddWorkText(const string & Text, int Index)
{
	CTime Time = CTime::GetTickCount();

	wstring Temp2;

	Temp2 += to_wstring(Time.GetYear());
	Temp2 += L"년 ";
	Temp2 += to_wstring(Time.GetMonth());
	Temp2 += L"월 ";
	Temp2 += to_wstring(Time.GetDay());
	Temp2 += L"일 ";
	Temp2 += to_wstring(Time.GetHour());
	Temp2 += L"시 ";
	Temp2 += to_wstring(Time.GetMinute());
	Temp2 += L"분 ";
	Temp2 += to_wstring(Time.GetSecond());
	Temp2 += L"초 ";

	wstring Temp = CA2W(Text.c_str());
	m_WorkList.InsertString(Index, Temp.c_str());
	ExcelManager::Get()->WriteData("EditorLog", 0, m_TextCount, Text);
	ExcelManager::Get()->WriteData("EditorLog", 6, m_TextCount, Temp2);

	m_TextCount++;
}

void EditorForm::AddWorkText(const CString & Text, int Index)
{
	CTime Time = CTime::GetTickCount();
	string Temp3;

	Temp3 += to_string(Time.GetYear());
	Temp3 += "년 ";
	Temp3 += to_string(Time.GetMonth());
	Temp3 += "월 ";
	Temp3 += to_string(Time.GetDay());
	Temp3 += "일 ";
	Temp3 += to_string(Time.GetHour());
	Temp3 += "시 ";
	Temp3 += to_string(Time.GetMinute());
	Temp3 += "분 ";
	Temp3 += to_string(Time.GetSecond());
	Temp3 += "초 ";

	string	Temp = CT2CA(Text);
	wstring Temp2 = CA2W(Temp.c_str());

	m_WorkList.InsertString(Index, Temp2.c_str());
	ExcelManager::Get()->WriteData("EditorLog", 0, m_TextCount, Text);
	ExcelManager::Get()->WriteData("EditorLog", 6, m_TextCount, Temp3);

	m_TextCount++;
}

void EditorForm::AddWorkText(wchar_t * Text, int Index)
{
	wstring Temp = Text;
	AddWorkText(Temp, Index);
}

void EditorForm::AddWorkText(char * Text, int Index)
{
	string Temp = Text;
	AddWorkText(Temp, Index);
}




int EditorForm::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFormView::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}


void EditorForm::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

}

void EditorForm::OnEnChangeScalex()
{
	UpdateData(TRUE);



	UpdateData(FALSE);
}

void EditorForm::OnEnChangeScaley()
{
	UpdateData(TRUE);


	UpdateData(FALSE);
}

void EditorForm::OnEnChangeScalez()
{
	UpdateData(TRUE);

	UpdateData(FALSE);
}

void EditorForm::OnEnChangeRotationx()
{
	UpdateData(TRUE);

	UpdateData(FALSE);
}

void EditorForm::OnEnChangeRotationy()
{
	UpdateData(TRUE);


	UpdateData(FALSE);
}

void EditorForm::OnEnChangeRotationz()
{
	UpdateData(TRUE);


	UpdateData(FALSE);
}

void EditorForm::OnEnChangePositionx()
{
	UpdateData(TRUE);


	UpdateData(FALSE);
}

void EditorForm::OnEnChangePositiony()
{
	UpdateData(TRUE);


	UpdateData(FALSE);
}

void EditorForm::OnEnChangePositionz()
{
	UpdateData(TRUE);

	UpdateData(FALSE);
}


//컨트롤박스
////////////////////////////////////////////////////////////////////////
void EditorForm::OnCbnSelchangeTileselect()
{
	UpdateData(TRUE);



	UpdateData(FALSE);
}

void EditorForm::OnCbnSelchangeTileoptionselect()
{
	UpdateData(TRUE);



	UpdateData(FALSE);
}

void EditorForm::OnCbnSelchangeTileimageselect()
{
	UpdateData(TRUE);


	UpdateData(FALSE);
}

void EditorForm::OnBnClickedTilecreatebutton()
{
	UpdateData(TRUE);
	UpdateData(FALSE);
}

void EditorForm::OnBnClickedColorsave()
{
	UpdateData(TRUE);

	UpdateData(FALSE);
}


void EditorForm::OnEnChangeColorr()
{
	UpdateData(TRUE);



	UpdateData(FALSE);
}

void EditorForm::OnEnChangeColorg()
{
	UpdateData(TRUE);



	UpdateData(FALSE);
}

void EditorForm::OnEnChangeColorb()
{
	UpdateData(TRUE);



	UpdateData(FALSE);
}

void EditorForm::OnEnChangeColora()
{
	UpdateData(TRUE);



	UpdateData(FALSE);
}

void EditorForm::OnEnChangeTilecountx()
{
	UpdateData(TRUE);


	UpdateData(FALSE);
}

void EditorForm::OnEnChangeTilecounty()
{
	UpdateData(TRUE);


	UpdateData(FALSE);
}

void EditorForm::OnEnChangeTilesizex()
{
	UpdateData(TRUE);


	UpdateData(FALSE);
}

void EditorForm::OnEnChangeTilesizey()
{
	UpdateData(TRUE);



	UpdateData(FALSE);
}

void EditorForm::OnEnChangeTagname()
{
	UpdateData(TRUE);



	UpdateData(FALSE);
}


void EditorForm::OnBnClickedTileclear()
{
	UpdateData(TRUE);


	UpdateData(FALSE);
}

void EditorForm::OnBnClickedLineon()
{
	UpdateData(TRUE);



	UpdateData(FALSE);
}


void EditorForm::OnBnClickedTileload()
{
	UpdateData(TRUE);



	UpdateData(FALSE);
}

void EditorForm::OnBnClickedTilesave()
{
	UpdateData(TRUE);

	UpdateData(FALSE);
}

void EditorForm::OnDraw(CDC* pDC)
{

}

void EditorForm::OnEnChangeCreatetilecount()
{
	UpdateData(TRUE);



	UpdateData(FALSE);
}
