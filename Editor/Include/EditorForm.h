#pragma once

// EditorForm �� ���Դϴ�.
JEONG_USING
class EditorForm : public CFormView
{
	DECLARE_DYNCREATE(EditorForm)

protected:
	EditorForm();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
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
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
};


