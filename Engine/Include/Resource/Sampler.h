#pragma once
#include "../RefCount.h"

JEONG_BEGIN
//샘플러는 필터링방법을 정의해 주는것이다. 
//필터링이란
//이미지의 확대 축소가 거의 대부분 일어날 수 밖에 없기 때문에 이미지 확대 축소시 깨지는 것을
//어느정도로 보정해주느냐를 설정해주는 것.
class Sampler : public RefCount
{
public:
	//어떻게 필터링 할 것인가에 대한 방법 (퀄리티가 높을수록 느리다)
	//비등방성 = 하급
	//선형필터링 = 중급
	//포인트필터링 = 고급

	//WRAP는 바둑판식처럼 1을초과해도 1~2의 UV를 가질때 이미지가 그대로 출력되는 것.
	bool CreateSampler(const string& TagName, D3D11_FILTER eFilter = D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_MODE eU = D3D11_TEXTURE_ADDRESS_WRAP, D3D11_TEXTURE_ADDRESS_MODE eV = D3D11_TEXTURE_ADDRESS_WRAP, D3D11_TEXTURE_ADDRESS_MODE eW = D3D11_TEXTURE_ADDRESS_WRAP);
	void SetSamplerState(int RegisterNumber);

private:
	//위의 것을 설정해주고 디바이스나 컨텍스트로 보내줄 변수가 필요하다.
	ID3D11SamplerState* m_SamplerState;

private:
	Sampler();
	~Sampler();
	
public:
	friend class ResourceManager;
};

JEONG_END