#include "../stdafx.h"
#include "Shader.h"
#include "../PathManager.h"
#include "../Device.h"

JEONG_USING

JEONG::Shader::Shader()
 : m_VsBlob(NULLPTR), m_PsBlob(NULLPTR), m_Vs(NULLPTR), m_Ps(NULLPTR)
{	
}

JEONG::Shader::~Shader()
{
	SAFE_RELEASE(m_VsBlob);
	SAFE_RELEASE(m_PsBlob);
	SAFE_RELEASE(m_Vs);
	SAFE_RELEASE(m_Ps);
}

void JEONG::Shader::SetShader()
{
	Device::Get()->GetContext()->VSSetShader(m_Vs, NULLPTR, 0);
	Device::Get()->GetContext()->PSSetShader(m_Ps, NULLPTR, 0);
}

bool JEONG::Shader::LoadShader(const string & shaderName, const TCHAR * FileName, char * Entry[ST_MAX], const string & PathKey)
{
	m_ShaderName = shaderName;
	SetTag(shaderName);

	if (LoadVertexShader(FileName, Entry[ST_VERTEX], PathKey) == false)
		return false;

	if (LoadPixelShader(FileName, Entry[ST_PIXEL], PathKey) == false)
		return false;

	return true;
}

bool JEONG::Shader::LoadVertexShader(const TCHAR * FileName, char * Entry, const string & PathKey)
{
	const TCHAR* pPath = PathManager::Get()->FindPath(PathKey);

	wstring	FullPath;

	if (pPath != NULLPTR)
		FullPath = pPath;
	FullPath += FileName;

	DWORD Flag = 0;

#ifdef _DEBUG
	Flag = D3DCOMPILE_DEBUG;
#endif 
	//쉐이더 컴파일 중 에러메세지를 내보내기 위함.
	ID3DBlob* ErrorMessage = NULLPTR;

	//HLSL을 컴파일한다.
	if (FAILED(D3DCompileFromFile(FullPath.c_str(), NULLPTR, D3D_COMPILE_STANDARD_FILE_INCLUDE, Entry, "vs_5_0", Flag, 0, &m_VsBlob, &ErrorMessage)))
	{
		OutputDebugStringA((char*)ErrorMessage->GetBufferPointer());
		TrueAssert(true);
		return false;
	}

	//Blob에 컴파일된 HLSL코드를 사용하여 버텍스쉐이더 객체를 만들어낸다.
	if (FAILED(Device::Get()->GetDevice()->CreateVertexShader(m_VsBlob->GetBufferPointer(), m_VsBlob->GetBufferSize(), NULLPTR, &m_Vs)))
	{
		TrueAssert(true);
		return false;
	}

	return true;
}

bool JEONG::Shader::LoadPixelShader(const TCHAR * FileName, char * Entry, const string & PathKey)
{
	const TCHAR* pPath = PathManager::Get()->FindPath(PathKey);

	wstring	FullPath;

	if (pPath != NULLPTR)
		FullPath = pPath;
	FullPath += FileName;

	DWORD Flag = 0;

#ifdef _DEBUG
	Flag = D3DCOMPILE_DEBUG;
#endif
	//쉐이더 컴파일 중 에러메세지를 내보내기 위함.
	ID3DBlob* ErrorMessage = NULLPTR;

	//HLSL을 컴파일한다.
	if (FAILED(D3DCompileFromFile(FullPath.c_str(), NULLPTR, D3D_COMPILE_STANDARD_FILE_INCLUDE, Entry, "ps_5_0", Flag, 0, &m_PsBlob, &ErrorMessage)))
	{
		OutputDebugStringA((char*)ErrorMessage->GetBufferPointer());
		return false;
	}

	//Blob에 컴파일된 HLSL코드를 사용하여 버텍스쉐이더 객체를 만들어낸다.
	if (FAILED(Device::Get()->GetDevice()->CreatePixelShader(m_PsBlob->GetBufferPointer(), m_PsBlob->GetBufferSize(), NULLPTR, &m_Ps)))
		return false;

	return true;
}