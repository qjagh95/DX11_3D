#include "stdafx.h"
#include "BineryReader.h"

JEONG_USING

BineryRead::BineryRead(const string& FileName)
{
	wstring Temp;
	Temp = PathManager::Get()->FindPath(DATA_PATH);

	if (Temp.empty() == true)
		return;

	Temp += CA2W(FileName.c_str());
	string Temp2 = CW2A(Temp.c_str());

	m_ReadFile.open(Temp2.c_str(), ios::binary);
}

BineryRead::BineryRead(const wstring& FileName)
{
	wstring Temp;
	Temp = PathManager::Get()->FindPath(DATA_PATH);

	if (Temp.empty() == true)
		return;

	Temp += FileName;
	m_ReadFile.open(Temp.c_str());
}

BineryRead::~BineryRead()
{
	m_ReadFile.close();
}

bool BineryRead::ReadBool()
{
	int Temp = -1;

	m_ReadFile.read((char*)&Temp, sizeof(bool));

	if (Temp == -1)
		TrueAssert(true);

	return (bool)Temp;
}

int BineryRead::ReadInt()
{
	int Temp = INT_MIN;

	m_ReadFile.read((char*)&Temp, sizeof(int));

	if (Temp == INT_MIN)
		TrueAssert(true);

	return Temp;
}

float BineryRead::ReadFloat()
{
	float Temp = (float)INT_MIN;

	m_ReadFile.read((char*)&Temp, sizeof(float));

	if (Temp == (float)INT_MIN)
		TrueAssert(true);

	return Temp;
}

double BineryRead::ReadDouble()
{
	double Temp = (double)INT_MIN;

	m_ReadFile.read((char*)&Temp, sizeof(double));

	if (Temp == (double)INT_MIN)
		TrueAssert(true);

	return Temp;
}

Vector2 BineryRead::ReadVector2()
{
	Vector2 Temp = Vector2((float)INT_MIN, (float)INT_MIN);

	m_ReadFile.read((char*)&Temp, sizeof(Vector2));

	if (Temp == (float)INT_MIN)
		TrueAssert(true);

	return Temp;
}

Vector3 BineryRead::ReadVector3()
{
	Vector3 Temp = Vector3((float)INT_MIN, (float)INT_MIN, (float)INT_MIN);

	m_ReadFile.read((char*)&Temp, sizeof(Vector3));

	if (Temp == (float)INT_MIN)
		TrueAssert(true);

	return Temp;
}

Vector4 BineryRead::ReadVector4()
{
	Vector4 Temp = Vector4((float)INT_MIN, (float)INT_MIN, (float)INT_MIN, (float)INT_MIN);

	m_ReadFile.read((char*)&Temp, sizeof(Vector4));

	if (Temp == (float)INT_MIN)
		TrueAssert(true);

	return Temp;
}

string BineryRead::ReadString()
{
	char getString[255] = {};
	size_t Datalen;

	m_ReadFile.read((char*)&Datalen, sizeof(size_t));
	m_ReadFile.read(getString, Datalen);

	return string(getString);
}

size_t BineryRead::ReadSizeT()
{
	size_t Data;
	m_ReadFile.read((char*)&Data, sizeof(Data));

	return Data;
}

wstring BineryRead::ReadWString()
{
	char getString[255];
	size_t Datalen;

	m_ReadFile.read((char*)&Datalen, sizeof(size_t));
	m_ReadFile.read(getString, Datalen);

	wstring ReturnString = CA2W(getString);

	return ReturnString;
}

void BineryRead::ReadData(bool & Data)
{
	m_ReadFile.read((char*)&Data, sizeof(bool));
}

void BineryRead::ReadData(int & Data)
{
	m_ReadFile.read((char*)&Data, sizeof(int));
}

void BineryRead::ReadData(float & Data)
{
	m_ReadFile.read((char*)&Data, sizeof(float));
}

void BineryRead::ReadData(double & Data)
{
	m_ReadFile.read((char*)&Data, sizeof(double));
}

void BineryRead::ReadData(Vector2 & Data)
{
	m_ReadFile.read((char*)&Data, sizeof(Vector2));
}

void BineryRead::ReadData(Vector3 & Data)
{
	m_ReadFile.read((char*)&Data, sizeof(Vector3));
}

void BineryRead::ReadData(Vector4 & Data)
{
	m_ReadFile.read((char*)&Data, sizeof(Vector4));
}

void BineryRead::ReadData(string & Data)
{
	size_t DataLen;
	char getData[255] = {};

	m_ReadFile.read((char*)&DataLen, sizeof(size_t));
	m_ReadFile.read(getData, DataLen);

	Data = getData;
}

void BineryRead::ReadData(wstring & Data)
{
	size_t DataLen;
	char getData[255] = {};

	m_ReadFile.read((char*)&DataLen, sizeof(size_t));
	m_ReadFile.read(getData, DataLen);

	Data = CA2W(getData);
}

void BineryRead::ReadData(size_t & Data)
{
	m_ReadFile.read((char*)&Data, sizeof(Data));
}

void BineryRead::ReadData(Matrix & Data)
{
	m_ReadFile.read((char*)&Data, sizeof(float) * 16);
}

void BineryRead::ReadData(char* Data)
{
	size_t Lenth = -1;

	m_ReadFile.read((char*)Lenth, sizeof(size_t));
	m_ReadFile.read(Data, Lenth);
}

void BineryRead::ReadData(wchar_t* Data)
{
	size_t Lenth = -1;

	m_ReadFile.read((char*)Lenth, sizeof(size_t));
	m_ReadFile.read((char*)Data, Lenth);
}

void BineryRead::ReadData(void * Data, size_t Size)
{
	m_ReadFile.read((char*)&Data, Size);
}
