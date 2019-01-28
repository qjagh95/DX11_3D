#pragma once
JEONG_BEGIN

class Shader;
class JEONG_DLL ShaderManager
{
public:
	bool Init();
	bool LoadShader(const string& KeyName, const TCHAR* FileName, char* Entry[ST_MAX], const string& PathKey = SHADER_PATH);
	//1.쉐이더파일 시맨틱 이름 2. 인덱스 3. 포맷 4. 바이트사이즈 5.디폴트값 셋팅
	void AddInputElement(char* Semantic, int Index, DXGI_FORMAT Format, int Size, int InputSlot = 0, D3D11_INPUT_CLASSIFICATION eInputClass = D3D11_INPUT_PER_VERTEX_DATA, int InstanceStepRate = 0);
	bool CreateInputLayOut(const string& InputLayoutKey, const string& ShaderKey);
	bool CreateCBuffer(const string& KeyName, int BufferSize, int RegisterNumber,int ShaderType);
	bool UpdateCBuffer(const string& KeyName, void *Info);

	CBuffer* FindCBuffer(const string& KeyName);
	Shader* FindShader(const string& KeyName);
	ID3D11InputLayout* FindInputLayOut(const string& KeyName);

private:
	unordered_map<string, Shader*> m_ShaderMap;
	//InputLayOut = VertexShader에 Vertex정보를 공급하는 방법이 들어있다.
	unordered_map<string, ID3D11InputLayout*> m_LayOutMap;
	//InputLayout가 몇개가 생성될지 모르니 vector (시맨틱)
	vector<D3D11_INPUT_ELEMENT_DESC> m_vecInputElement;
	//쉐이더로 전해줄 버퍼공간
	unordered_map<string, CBuffer*> m_CBufferMap;

	int m_InputLayOutSize;

public:
	CLASS_IN_SINGLE(ShaderManager)
};

JEONG_END

