
// MainFrm.h : CMainFrame 클래스의 인터페이스
//

#pragma once

class EditorView;
class EditorForm;

class MainFrame : public CFrameWnd
{
	
protected: // serialization에서만 만들어집니다.
	MainFrame();
	DECLARE_DYNCREATE(MainFrame)

// 특성입니다.
public:

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	EditorView* GetEditorView() const { return m_EditorView; }
	EditorForm* GetEditorForm() const { return m_EditorForm; }

	CSplitterWnd m_SplitWindow;

	MainFrame* GetMainframe() { return this; }
// 구현입니다.
public:
	virtual ~MainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  
	//분할윈도우 (동적분할과 정적분할이 있음)

	EditorView* m_EditorView;
	EditorForm* m_EditorForm;


// 생성된 메시지 맵 함수
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL DestroyWindow();
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
};


