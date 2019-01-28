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
	wstring ReadWString();

	void ReadData(bool& Data);
	void ReadData(int& Data);
	void ReadData(float& Data);
	void ReadData(double& Data);
	void ReadData(Vector2& Data);
	void ReadData(Vector3& Data);
	void ReadData(Vector4& Data);
	void ReadData(string& Data);
	void ReadData(wstring& Data);

private:
	ifstream m_ReadFile;

public:
	BineryRead(const string& FileName);
	BineryRead(const wstring& FileName);
	~BineryRead();
};

JEONG_END

