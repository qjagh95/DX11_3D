#pragma once

#include <Windows.h>
#include <list>
#include <vector>
#include <unordered_map>
#include <string>
#include <crtdbg.h>
#include <conio.h>
#include <assert.h>
#include <array>
#include <thread>
#include <mutex>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <atlstr.h>
#include <process.h>

//DX11부터는 포함디렉터리 라이브러리 디렉터리 추가없이 바로 헤더추가가 가능하다.
//이전에는 프로젝트 템플릿(VS기능)을 이용하여 프로젝트를 먼저 만들어놓고 추가해서 사용했다.
#include <d3d11.h>
#include <d3dcompiler.h> //shader를 컴파일하는 기능을 제공한다.
#include <d2d1.h>
#include <dwrite.h>
#include <dxgidebug.h>
#include <dinput.h>

#ifdef GUI_USING

///Excel
#include "enum.h"
#include "IBookT.h"
#include "IFontT.h"
#include "IFormatT.h"
#include "ISheetT.h"
#include "setup.h"
#include "libxl.h"
#pragma comment(lib, "libxl")

#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"

#endif

#include <wrl/client.h>
#include "tinyxml2.h"

#include "MathHeader.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix.h"

#include <Audio.h>

#pragma comment(lib, "dxguid")
#pragma comment(lib, "d3d11")
#pragma comment(lib, "d3dcompiler")
//DWrite
#pragma comment(lib, "d2d1")
#pragma comment(lib, "dwrite")
//DInput
#pragma comment(lib, "dinput8")
#pragma comment(lib, "dxguid")

#pragma warning(disable:4099)

///Math
#include "Macro.h"
#include "Flag.h"
#include "Type.h"

#define JEONG_BEGIN namespace JEONG {
#define JEONG_END }
#define JEONG_USING using namespace JEONG;
#define TrueAssert(Var) assert(!(Var))

using namespace libxl;

//#ifdef _DEBUG
//#define new new(_CLIENT_BLOCK,__FILE__,__LINE__)
//#endif // DEBUG


#include "FontManager.h"
#include "Device.h"
#include "PathManager.h"
#include "TimeManager.h"
#include "CollsionManager.h"
#include "KeyInput.h"
#include "SlotManager.h"
#include "GameObject.h"
#include "SoundManager.h"
#include "StaticManager.h"
#include "ExcelManager.h"
#include "BineryWriter.h"
#include "BineryReader.h"
#include "GUIManager.h"