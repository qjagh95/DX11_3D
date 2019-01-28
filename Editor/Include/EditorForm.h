#pragma once

// EditorForm 폼 뷰입니다.
JEONG_USING
class EditorForm : public CFormView
{
	DECLARE_DYNCREATE(EditorForm)

protected:
	EditorForm();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~EditorForm();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_EDIT };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	Vector3 m_Scale;
	Vector3 m_Rot;
	Vector3 m_Pos;
	int m_ScaleX;
	int m_ScaleY;
	int m_ScaleZ;
	int m_RotationX;
	int m_RotationY;
	int m_RotationZ;
	int m_PosX;
	int m_PosY;
	int m_PosZ;

	CString m_TagName;
	int m_TileCountX;
	int m_TileCountY;
	int m_TileSizeX;
	int m_TileSizeY;
	int m_StartPosX;
	int m_StartPosY;

	GameObject* m_StageObject;
	Transform_Com* m_StageTransform;

	int m_BackColorR;
	int m_BackColorG;
	int m_BackColorB;
	int m_BackColorA;

	BOOL m_isLine;

	void AddWorkText(const wstring& Text, int Index = 0);
	void AddWorkText(const string& Text, int Index = 0);
	void AddWorkText(const CString& Text, int Index = 0);
	void AddWorkText(wchar_t* Text, int Index = 0);
	void AddWorkText(char* Text, int Index = 0);

	CComboBox m_TileTypeBox;
	CComboBox m_TileOptionBox;
	CComboBox m_TileImageBox;
	CListBox m_WorkList;

	CEdit m_StartPosXControl;
	CEdit m_StartPosYControl;
	CEdit m_StartPosZControl;

	CImage m_TileImage;
	wstring m_Path;

	int m_TextCount;
	bool m_isImageLoad;

	CString m_ImageName;
	int m_CreateTileCount;


	virtual void OnDraw(CDC* pDC);
	virtual void OnInitialUpdate();
	afx_msg void OnEnChangeScalex();
	afx_msg void OnEnChangeRotationx();
	afx_msg void OnEnChangePositionx();
	afx_msg void OnEnChangeScaley();
	afx_msg void OnEnChangeRotationy();
	afx_msg void OnEnChangePositiony();
	afx_msg void OnEnChangeScalez();
	afx_msg void OnEnChangeRotationz();
	afx_msg void OnEnChangePositionz();
	afx_msg void OnCbnSelchangeTileselect();
	afx_msg void OnCbnSelchangeTileoptionselect();
	afx_msg void OnCbnSelchangeTileimageselect();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnBnClickedTilecreatebutton();

	afx_msg void OnBnClickedColorsave();

	afx_msg void OnEnChangeColorr();
	afx_msg void OnEnChangeColorg();
	afx_msg void OnEnChangeColorb();
	afx_msg void OnEnChangeColora();
	afx_msg void OnEnChangeTilecountx();
	afx_msg void OnEnChangeTilecounty();
	afx_msg void OnEnChangeTilesizex();
	afx_msg void OnEnChangeTilesizey();
	afx_msg void OnEnChangeTagname();
	afx_msg void OnBnClickedTileclear();

	afx_msg void OnBnClickedLineon();
	afx_msg void OnBnClickedTileload();
	afx_msg void OnBnClickedTilesave();
	afx_msg void OnEnChangeCreatetilecount();
};


