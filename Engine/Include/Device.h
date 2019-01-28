#pragma once
JEONG_BEGIN

class JEONG_DLL Device
{
public:
	bool Init(HWND hWnd, unsigned int Width, unsigned int Height, bool isWindowMode);
	void Clear(float ClearColor[4]);
	void Present();
	ID3D11Device* GetDevice() const { return m_Device; }
	ID3D11DeviceContext* GetContext() const { return m_Context; }
	IDXGISwapChain* GetSwapChain() const { return m_SwapChain; }
	WinSize GetWinSize() const { return m_WinSize; }
	Vector2 GetWindowToDeviceRatio() const;
	ID2D1RenderTarget* Get2DRenderTarget() const { return m_2DTarget; }

private:
	//Com - Component Object의 약자 - 전부 RefCount방식으로 돌아간다.
	//Device같은것들이 Com(IUNOWS)을 상속받는다(RefCount)

	//DX의 더블버퍼링방식은 페이지플리핑을 사용한다.
	//페이지플리핑 - 백버퍼에 그린다->백퍼버가 주 표면이되고 주 표면이 백버퍼가된다.계속 왔다갔다하는 방식. (리스트의 포인터연산과 비슷)
	//복사하는 작업이 없기때문에 더 빠르다 (API의 BitBlt - 픽셀을 복사)

	//11은 멀티쓰레드랜더링이 가능하다는점이 강력함.
	//멀티쓰레드 - 병렬처리프로그래밍 1번함수조금 2번함수조금 실행

	//11부터는 디바이스 컨텍스트가 같이있다

	//1. 이미디어택컨텍스트 - 일반적으로 사용(즉시라는뜻)
	//2. 디퍼드컨텍스트 - 멀티쓰레드

	//9의 디바이스는 모든걸 담당하다보니 11부터는 나눔
	//디바이스는 보통 버퍼생성 컨텍스트를 이용하여 상태설정을한다.

	//스왑체인 - 백버퍼를 관리해줄놈(윈도우를 늘리면 스왑체인이 늘어나고 이것을 활용하면 측면화면, 정면화면, 후면화면 등을 볼 수 있다)

	//반드시 WinSize와 DX디바이스 사이즈가 같을 필요는 없다.
	//Window위에 얇은 막을 씌운 후 그 위에 그린다고 생각하면 된다.

	ID3D11Device* m_Device;					///디바이스 - 그래픽카드와 연결하여 명령을 대신 내려주는 놈.
	ID3D11DeviceContext* m_Context;			///디바이스 컨텍스트 - 디바이스의 상태설정을 담당한다
	IDXGISwapChain* m_SwapChain;			///스왑체인 - 백버퍼를 관리한다.
	ID3D11RenderTargetView* m_TargerView;   ///랜더타겟뷰 - 출력할 랜더타겟 (디바이스에 등록해놓을 것) 그냥 백퍼버
	ID3D11DepthStencilView* m_DepthView;    ///깊이스탠실뷰
	WinSize m_WinSize;
	HWND m_Hwnd;

#ifdef _DEBUG
	//IDXGIDebug* dxgi_debug;
	//HMODULE asd;
#endif

	//2D Interface
	ID2D1RenderTarget* m_2DTarget;
	ID2D1Factory* m_2DFactory;

public:
	CLASS_IN_SINGLE(Device)
};

JEONG_END
