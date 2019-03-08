#pragma once

JEONG_BEGIN

class JEONG_DLL BineryRead
{
public:
	bool ReadBool();
	int ReadInt();
	float ReadFloat();
	double ReadDouble();
	Vector2 ReadVector2();
	Vector3 ReadVector3();
	Vector4 ReadVector4();
	string ReadString();
	size_t ReadSizeT();
	wstring ReadWString();
	Matrix ReadData();
	void* ReadUserData(size_t size);

	void ReadData(bool& Data);
	void ReadData(int& Data);
	void ReadData(float& Data);
	void ReadData(double& Data);
	void ReadData(Vector2& Data);
	void ReadData(Vector3& Data);
	void ReadData(Vector4& Data);
	void ReadData(string& Data);
	void ReadData(wstring& Data);
	void ReadData(size_t& Data);
	void ReadData(Matrix& Data);
	void ReadData(char* Data);
	void ReadData(wchar_t* Data);
	void ReadData(void* Data, size_t Size);

private:
	ifstream m_ReadFile;

public:
	BineryRead(const TCHAR* FullPath);
	BineryRead(const char* FullPath);
	BineryRead(const string& FileName);
	BineryRead(const wstring& FileName);
	~BineryRead();
};

JEONG_END

