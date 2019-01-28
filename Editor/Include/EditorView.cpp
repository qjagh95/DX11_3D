
// EditorView.cpp : EditorView 클래스의 구현
//

#include "EditorHeader.h"
#include "EditScene.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "Editor.h"
#endif

#include "EditorDoc.h"
#include "EditorView.h"
#include "EditorForm.h"
#include <Component/FreeCamera_Com.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//EditorView

IMPLEMENT_DYNCREATE(EditorView, CView)

BEGIN_MESSAGE_MAP(EditorView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()

//EditorView 생성/소멸

EditorView::EditorView()
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

EditorView::~EditorView()
{
}

BOOL EditorView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}


void EditorView::OnDraw(CDC* /*pDC*/)
{
	EditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if (!pDoc)
		return;
}

BOOL EditorView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void EditorView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void EditorView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// EditorView 진단

#ifdef _DEBUG
void EditorView::AssertValid() const
{
	CView::AssertValid();
}

void EditorView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

EditorDoc* EditorView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(EditorDoc)));
	return (EditorDoc*)m_pDocument;
}
#endif //_DEBUG

//이 클래스에서 Window를 띄운다.

//TODO
// EditorView 메시지 처리기
void EditorView::OnInitialUpdate()
{
	CView::OnInitialUpdate();
}

BOOL EditorView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	MainFrame* mainFrame = (MainFrame*)AfxGetMainWnd();
	EditorForm*	editorForm = mainFrame->GetEditorForm();

	Scene* getScene = SceneManager::Get()->GetCurScene();
	Camera_Com* getCamera = getScene->GetMainCamera();

	Vector3 CameraNormal = Vector3::Nomallize(getCamera->GetTransform()->GetWorldPos());
	Vector3 MouseNormal = Vector3::Nomallize(KeyInput::Get()->GetMouseWorldPos());

	Vector3 CameraPos = getCamera->GetTransform()->GetWorldPos();
	Vector3 MousePos = KeyInput::Get()->GetMouseWorldPos();

	SAFE_RELEASE(getScene);

	return CView::OnMouseWheel(nFlags, zDelta, pt);
}
