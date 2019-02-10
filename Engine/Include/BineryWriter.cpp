#include "stdafx.h"
#include "BineryWriter.h"

JEONG_USING

JEONG::BineryWrite::BineryWrite(const string& FileName)
{
	wstring Temp;
	Temp = PathManager::Get()->FindPath(DATA_PATH);

	if (Temp.empty() == true)
		return;

	Temp += CA2W(FileName.c_str());
	string Temp2 = CW2A(Temp.c_str());

	m_WriteFile = ofstream(Temp2.c_str(), ios_base::binary);
}

JEONG::BineryWrite::BineryWrite(const wstring& FileName)
{
	wstring Temp;
	Temp = PathManager::Get()->FindPath(DATA_PATH);

	if (Temp.empty() == true)
		return;

	Temp += FileName;

	m_WriteFile.open(Temp.c_str(), ios::binary);
}

JEONG::BineryWrite::~BineryWrite()
{
	m_WriteFile.close();
}

void JEONG::BineryWrite::WriteData(const bool& Data)
{
	m_WriteFile.write((char*)&Data, sizeof(bool));
}

void JEONG::BineryWrite::WriteData(const int& Data)
{
	m_WriteFile.write((char*)&Data, sizeof(int));
}

void JEONG::BineryWrite::WriteData(const float& Data)
{
	m_WriteFile.write((char*)&Data, sizeof(float));
}

void JEONG::BineryWrite::WriteData(const double& Data)
{
	m_WriteFile.write((char*)&Data, sizeof(double));
}

void JEONG::BineryWrite::WriteData(const size_t& Data)
{
	m_WriteFile.write((char*)&Data, sizeof(size_t));
}

void JEONG::BineryWrite::WriteData(const Vector2 & Data)
{
	m_WriteFile.write((char*)&Data, sizeof(Vector2));
}

void JEONG::BineryWrite::WriteData(const Vector3 & Data)
{
	m_WriteFile.write((char*)&Data, sizeof(Vector3));
}

void JEONG::BineryWrite::WriteData(const Vector4 & Data)
{
	m_WriteFile.write((char*)&Data, sizeof(Vector4));
}

void JEONG::BineryWrite::WriteData(const string & Data)
{
	size_t Datalen = Data.length();

	m_WriteFile.write((char*)&Datalen, sizeof(size_t));
	m_WriteFile.write(Data.c_str(), Datalen);
}

void JEONG::BineryWrite::WriteData(const wstring & Data)
{
	string Temp = CW2A(Data.c_str());
	size_t Datalen = Data.length();

	m_WriteFile.write((char*)&Datalen, sizeof(size_t));
	m_WriteFile.write(Temp.c_str(), Datalen);
}

void JEONG::BineryWrite::WriteData(const char * Data)
{
	string Temp = Data;
	size_t Datalen = Temp.length();

	m_WriteFile.write((char*)&Datalen, sizeof(size_t));
	m_WriteFile.write(Data, Datalen);
}

void JEONG::BineryWrite::WriteData(const wchar_t * Data)
{
	wstring Temp = Data;
	string Temp2 = CW2A(Temp.c_str());

	size_t Datalen = Temp2.length();

	m_WriteFile.write((char*)&Datalen, sizeof(size_t));
	m_WriteFile.write(Temp2.c_str(), Datalen);
}

void JEONG::BineryWrite::WriteData(const Matrix & Data)
{
	m_WriteFile.write((char*)&Data, sizeof(float) * 16);
}

void JEONG::BineryWrite::WriteData(const void * Data, size_t Size)
{
	m_WriteFile.write((char*)&Data, Size);
}

