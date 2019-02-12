#include "stdafx.h"
#include "FBXLoader.h"

JEONG_USING

FBXLoader::FBXLoader()
{
	/*

	1. Manager생성 - 최상위
	2. IOSetting 생성
	3. Importer 생성
	4. Scene생성

	Importer가 읽은 데이터를 Scene에 넘겨준다.
	Scene은 그래프구조 (트리)

	Scene에서 자식노드와 속성을 돌며 데이터 내 구조에 맞게 데이터를 입력한다.

	*/
}

FBXLoader::~FBXLoader()
{
}

