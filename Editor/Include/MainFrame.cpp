// MainFrm.cpp : MainFrame 클래스의 구현
//

#include "EditorHeader.h"
#include "Editor.h"
#include "EditorView.h"
#include "EditorForm.h"

#include "MainFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(MainFrame, CFrameWnd)

const int  iMaxUserToolbars = 10;
const UINT uiFirstUserToolBarId = AFX_IDW_CONTROLBAR_FIRST + 40;
const UINT uiLastUserToolBarId = uiFirstUserToolBarId + iMaxUserToolbars - 1;

BEGIN_MESSAGE_MAP(MainFrame, CFrameWnd)
	ON_WM_CREATE()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 상태 줄 표시기
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame 생성/소멸

MainFrame::MainFrame()
{
	// TODO: 여기에 멤버 초기화 코드를 추가합니다.
}

MainFrame::~MainFrame()
{
}

//윈도우프레임이 맨처음 만들어질때 들어오는 함수
int MainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

BOOL MainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;

	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	//메인프레임 가로크기
	cs.cx = 1800;
	cs.cy = 720;

	return TRUE;
}

#ifdef _DEBUG
void MainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void MainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG

//TODO
BOOL MainFrame::DestroyWindow()
{
	return CFrameWnd::DestroyWindow();
}


BOOL MainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	//창을 분할한다.
	//부모, 세로몇개 가로몇개?
	m_SplitWindow.CreateStatic(this, 1, 2);

	//분할된 각 창에 원하는 윈도우를 생성한다.
	m_SplitWindow.CreateView(0, 0, RUNTIME_CLASS(EditorView), CSize(1280, 720), pContext);
	m_SplitWindow.CreateView(0, 1, RUNTIME_CLASS(EditorForm), CSize(520, 720), pContext);
	
	//윈도우를 얻어오는 함수.
	m_EditorView = (EditorView*)m_SplitWindow.GetPane(0, 0);
	m_EditorForm = (EditorForm*)m_SplitWindow.GetPane(0, 1);

	return TRUE;
}