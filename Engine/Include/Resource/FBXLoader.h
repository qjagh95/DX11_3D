#pragma once
#include "fbxsdk.h"

JEONG_BEGIN

class JEONG_DLL FBXLoader
{
private:
	FBXLoader();
	~FBXLoader();

public:
	friend class Mesh;
};

JEONG_END