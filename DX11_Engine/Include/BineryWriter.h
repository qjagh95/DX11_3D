#pragma once
JEONG_BEGIN
class JEONG_DLL BineryWrite
{
public:
	void WriteData(const bool& Data);
	void WriteData(const int& Data);
	void WriteData(const float& Data);
	void WriteData(const double& Data);
	void WriteData(const Vector2& Data);
	void WriteData(const Vector3& Data);
	void WriteData(const Vector4& Data);
	void WriteData(const string& Data);
	void WriteData(const wstring& Data);
	void WriteData(const char* Data);
	void WriteData(const wchar_t* Data);

private:
	ofstream m_WriteFile;

public:
	BineryWrite(const string& FileName);
	BineryWrite(const wstring& FileName);
	~BineryWrite();
};

JEONG_END
