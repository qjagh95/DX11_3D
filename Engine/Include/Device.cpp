#include "stdafx.h"
#include "Device.h"

JEONG_USING

SINGLETON_VAR_INIT(JEONG::Device);

JEONG::Device::Device()
	:m_Device(NULLPTR),
	m_Context(NULLPTR), 
	m_SwapChain(NULLPTR),
	m_TargerView(NULLPTR),
	m_DepthView(NULLPTR), 
	m_Hwnd(NULLPTR),
	m_2DFactory(NULLPTR),
	m_2DTarget(NULLPTR)
{
}

JEONG::Device::~Device()
{
	SAFE_RELEASE(m_DepthView);
	SAFE_RELEASE(m_TargerView);
	SAFE_RELEASE(m_SwapChain);

	if (m_Context != NULLPTR)
		m_Context->ClearState();

	SAFE_RELEASE(m_Context);
	SAFE_RELEASE(m_Device);

	SAFE_RELEASE(m_2DFactory);
	SAFE_RELEASE(m_2DTarget);

#ifdef _DEBUG
	//dxgi_debug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_DETAIL);
	//SAFE_RELEASE(dxgi_debug);
	//FreeLibrary(asd);
#endif
}

bool JEONG::Device::Init(HWND hWnd, unsigned int Width, unsigned int Height, bool isWindowMode)
{
	m_Hwnd = hWnd;
	m_WinSize.Width = Width;
	m_WinSize.Height = Height;

	UINT Flag = 0;

#ifdef _DEBUG
	Flag = D3D11_CREATE_DEVICE_DEBUG;

	//asd = LoadLibrary(L"DXGIDebug.dll");
	//auto qwe = reinterpret_cast<HRESULT(*)(REFIID, void**)>(GetProcAddress(asd, "DXGIGetDebugInterface"));
	//qwe(__uuidof(IDXGIDebug), reinterpret_cast<void**>(&dxgi_debug));
	 
#endif
	//DWrite는 2D에서만 동작을하는데 3D에서도 동작할 수 있도록 옵션설정.
	Flag |= D3D11_CREATE_DEVICE_BGRA_SUPPORT;

	D3D_FEATURE_LEVEL eLevel1 = D3D_FEATURE_LEVEL_11_0;
	D3D_FEATURE_LEVEL eLevel2 = D3D_FEATURE_LEVEL_11_0;

	//스왑체인은 페이지플리핑 역할을 한다. 그래서 백버퍼를 관리하는 역할을 한다. 이 정보를 이용하여 백버퍼를 만들어낸다
	DXGI_SWAP_CHAIN_DESC SwapDesc = {};
	SwapDesc.BufferDesc.Width = Width;
	SwapDesc.BufferDesc.Height = Height;
	SwapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;					 ///RGBA의값들을 각각 0~1사이로 사용하겠다.
	SwapDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;				 ///디폴트값 뭘어떻게하던 장치가정해준 것을 이길 수 없다
	SwapDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED; ///디폴드값
	SwapDesc.BufferDesc.RefreshRate.Numerator = 60;								 ///분자 (//모니터주사율// 무조건 어떻게하던 그래픽카드에서 모니터로)
	SwapDesc.BufferDesc.RefreshRate.Denominator = 1;							 ///분모 (초당 60번을 쏴주겠다)
	SwapDesc.BufferCount = 1;													 ///백버퍼의 갯수
	SwapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;						 ///출력용 랜더타겟(내가 출력할 대상)을 만들어낸다
	SwapDesc.OutputWindow = hWnd;												 ///어느윈도우대상으로??
	SwapDesc.SampleDesc.Count = 1;												 ///안티앨리어싱기능 (개느려서 안쓸꺼야)
	SwapDesc.SampleDesc.Quality = 0;											 ///직접 쉐이더로 만드는게 빨라 (하나는있다, 퀄리티0)
	SwapDesc.Windowed = isWindowMode;											 ///창모드 풀스크린모드 설정.
	SwapDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;								 ///옵션 설정에따라 다르다. 필요한것에 따라서 하자.
	//여기까지 백버퍼를 만들긴 한것이다.

	//1. 어댑터 타입 (그래픽카드가 2개이상일경우, 하지만 그런경우는 거의 없기때문에 NULLPTR)
	//2. 드라이버 타입 - 그래픽카드가 DX11을 지원하지 않을경우 CPU가 해주도록하는 설정 (DX11을 지원하지 않으면 느리다. (나중에 드라이버가속등을 사용하기위함.)
	//3. HMOUDLE - DirceX를 돌려주는 별도의 소프트웨어가 있다면 설정. == 의미없엉
	//4. Flag - 이 디바이스를 만들어낼때 어떤 옵션정보를 넣을것인가를 or연산으로 집어넣는다. (위 주석)
	//5. 피쳐레벨 - 장치지원수준 (11버전 지원이 안되면 ㅂ2ㅂ2)
	//6. SDK버전 - 고정값
	//나머지 변수들 더블포인터 주소값~
	if (FAILED(D3D11CreateDeviceAndSwapChain(NULLPTR, D3D_DRIVER_TYPE_HARDWARE, 0, Flag, &eLevel1, 1, D3D11_SDK_VERSION, &SwapDesc, &m_SwapChain, &m_Device, &eLevel2, &m_Context)))
	{
		TrueAssert(true);
		return false;
	}

	//스왑체인이 가지고있는 백버퍼를 출력병합기에 묶어줘야한다. 깊이버퍼도 같이묶어야한다.
	//뷰포트도 셋팅해줘야한다.

	ID3D11Texture2D* pBuffer = NULLPTR;										///com객체를 얻어오면 래퍼런스카운트가 +1 증가한다.
	//스왑체인에서 백버퍼를 뽑아온다.

	m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBuffer);	///__uuidof 클래스의 고유 식별번호를 알아오는 키워드
	//랜더타겟뷰에 어떤 버퍼를 지정해놓으면 이 뷰에 묶여있는 버퍼에다 출력을해준다.
	m_Device->CreateRenderTargetView(pBuffer, NULLPTR, &m_TargerView);
	
	SAFE_RELEASE(pBuffer);

	//백버퍼 자체는 스왑체인이 만들어지는 순간 같이 만들어진다.	거기서 백버퍼를 얻어올 것이다.
	//픽셀정보는 보통 텍스쳐에 저장한다.

	//깊이 정보를 설정한다. (백버퍼의 크기와 동일하다) 깊이정보는 텍스쳐2D로 설정한다.
	D3D11_TEXTURE2D_DESC DepthDesc = {};
	DepthDesc.Width = Width;
	DepthDesc.Height = Height;
	DepthDesc.ArraySize = 1;
	DepthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;			///이 텍스쳐를 만든용도는 깊이버퍼를 만든용도다.
	DepthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;       ///깊이에 24비트 스탠실에 8비트를 사용하겠다 라는뜻.
	DepthDesc.Usage = D3D11_USAGE_DEFAULT;					
	DepthDesc.MipLevels = 1;
	DepthDesc.SampleDesc.Count = 1;
	DepthDesc.SampleDesc.Quality = 0;

	//CPU설정은 Usage타입에따라서 다르다. Default는 CPU에서 접근불가.

	//Usage
	//1.DEFAULT - 일반적으로 많이쓰는 옵션 (비디오메모리에 언제든지 올려놓고쓴다. 단 수정만 가능하다. 갱신가능, 메모리읽기X)
	//2.IMMUTABLE - 완전폐쇄적 (처음 셋팅시 모든 정보를 버퍼에 셋팅을하고 출력용으로만 사용한다. 출력성능은 올라간다 데이터를 아예 바꾸지않겠다라면 ㄱㅊ)
	//3.DYNAMIC - 동적버퍼를만든다 실시간으로 CPU에서 접근해서 데이터를 갱신한다. (주로 파티클 에서쓴다) (차이 - CPU에 복사본을 만들어서 그것을 갱신해서 업데이트)
	//4.STAGING - 완전 오픈형. (출력이 되지않음. 데이터 저장용 버퍼.)

	//Texture2D Desc, 채워줄 픽셀정보, Texture2D 변수
	if (FAILED(m_Device->CreateTexture2D(&DepthDesc, NULLPTR, &pBuffer)))
	{
		TrueAssert(true);
		return false;
	}
	    
	//해당 버퍼에 깊이-스탠실 뷰를 만든다.
	if(FAILED(m_Device->CreateDepthStencilView(pBuffer, NULLPTR, &m_DepthView)))
	{
		TrueAssert(true);
		return false;
	}

	SAFE_RELEASE(pBuffer);

	//만들어준 타겟뷰와 뎁스뷰를 랜더링 파이프라인에 묶어준다. (Output Merser), 카운트와 포인터배열 즉 타겟뷰가 여러개일경우 사용
	m_Context->OMSetRenderTargets(1, &m_TargerView, m_DepthView); 
	//계속 타겟뷰에 랜더링 파이프라인에서 마지막단계에 거쳐올 데이터들을 기반으로 화면에 쏴줄것이다.

	D3D11_VIEWPORT ViewPort = {};
	//뷰포트의 화면크기를 지정한다.
	ViewPort.Width = (float)Width;
	ViewPort.Height = (float)Height;
	ViewPort.MaxDepth = 1;
	//컨텍스트에 뷰포트를 셋팅한다.  (갯수, 포인터배열) 
	m_Context->RSSetViewports(1, &ViewPort);
	//레스터라이저가 Depth판단까지 겸해서 한다.

	//DWrite사용을 위한 초기화
	//D2DFactory를 초기화한다.
	D2D1_FACTORY_OPTIONS tOption = {};
	tOption.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;

	//싱글쓰레드일때 3D렌더링 후 2D렌더링이돈다
	//멀티쓰레드일때 쓰레드를 하나 생성 후 3D와 2D렌더링 동시에 된다.
	if (FAILED(D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED, tOption, &m_2DFactory)))
		return false;

	// DirectX11 BackBuffer 타겟을 이용하여 2D 렌더링 타겟을 설정해준다.
	IDXGISurface* pBackSurface = NULLPTR;
	m_SwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackSurface));

	// 2D 렌더타겟을 생성하기 위한 옵션 설정
	D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_HARDWARE, D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED));

	if (FAILED(m_2DFactory->CreateDxgiSurfaceRenderTarget(pBackSurface, props, &m_2DTarget)))
		return false;

	SAFE_RELEASE(pBackSurface);
	return true;
}

void JEONG::Device::Clear(float ClearColor[4])
{
	//계속해서 화면을 지워줄것이다.
	m_Context->ClearRenderTargetView(m_TargerView, ClearColor);
	m_Context->ClearDepthStencilView(m_DepthView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void JEONG::Device::Present()
{
	m_SwapChain->Present(0, 0);
}

Vector2 JEONG::Device::GetWindowToDeviceRatio() const
{
	RECT DeviceSize;
	GetClientRect(m_Hwnd, &DeviceSize);

	return Vector2((float)m_WinSize.Width / (float)DeviceSize.right , (float)m_WinSize.Height/ (float)DeviceSize.bottom);
}
