// EditorForm.cpp : ���� �����Դϴ�.
//
#include "EditorHeader.h"
#include "Editor.h"
#include "EditorForm.h"
#include "afxwin.h"
#include <BineryReader.h>
#include <BineryWriter.h>


IMPLEMENT_DYNCREATE(EditorForm, CFormView)

EditorForm::EditorForm()
	: CFormView(IDD_DIALOG_EDIT)
{	
}

EditorForm::~EditorForm()
{
}

void EditorForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);

}

BEGIN_MESSAGE_MAP(EditorForm, CFormView)
END_MESSAGE_MAP()

// EditorForm �����Դϴ�.

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
