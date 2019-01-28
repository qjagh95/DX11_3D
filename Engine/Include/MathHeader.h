#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>

using namespace std;
using namespace DirectX;
using namespace DirectX::PackedVector;

#ifdef MATH_EXPORT
#define MATH_DLL __declspec(dllexport)
#else
#define MATH_DLL __declspec(dllimport)
#endif

#define TrueAssert(Type) assert(!(Type))

#define JEONG_PI 3.141592f

float MATH_DLL RadianToDegree(float radian);
float MATH_DLL DegreeToRadian(float degree);
int MATH_DLL RandomRange(int R1, int R2);

#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix.h"