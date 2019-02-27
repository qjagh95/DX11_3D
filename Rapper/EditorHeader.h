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
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <atlstr.h>
#include <process.h>

#include <d3d11.h>
#include <d3dcompiler.h> 
#include <d2d1.h>
#include <dwrite.h>
#include <dxgidebug.h>
#include <dinput.h>

#include <tinyxml2.h>

#include <MathHeader.h>
#include <Vector2.h>
#include <Vector3.h>
#include <Vector4.h>
#include <Matrix.h>

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
#include <Macro.h>
#include <Flag.h>
#include <Type.h>

#define JEONG_BEGIN namespace JEONG {
#define JEONG_END }
#define JEONG_USING using namespace JEONG;
#define TrueAssert(Var) assert(!(Var))

#include <Core.h>
#include <FontManager.h>
#include <Device.h>
#include <PathManager.h>
#include <TimeManager.h>
#include <CollsionManager.h>
#include <KeyInput.h>
#include <GameObject.h>
#include <StaticManager.h>
#include <BineryWriter.h>
#include <BineryReader.h>
#include <GUIManager.h>