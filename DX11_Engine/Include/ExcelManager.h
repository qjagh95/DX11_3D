#pragma once

JEONG_BEGIN

struct JEONG_DLL ExcelData
{
	Book* book;
	Sheet* sheet;

	ExcelData() { book = NULLPTR, sheet = NULLPTR; }
	~ExcelData() { book->release(); }
};

class JEONG_DLL ExcelManager
{
public:
	bool Init();
	
	void WriteData(const string& KeyName, const Vector2& CellNumber, const int& Data);
	void WriteData(const string& KeyName, const Vector2& CellNumber, const string& Data);
	void WriteData(const string& KeyName, const Vector2& CellNumber, const wstring& Data);
	void WriteData(const string& KeyName, const Vector2& CellNumber, const float& Data);
	void WriteData(const string& KeyName, const Vector2& CellNumber, const double& Data);
	void WriteData(const string& KeyName, const Vector2& CellNumber, const bool& Data);
	void WriteData(const string& KeyName, const Vector2& CellNumber, const Vector2& Data);
	void WriteData(const string& KeyName, const Vector2& CellNumber, const Vector3& Data);
	void WriteData(const string& KeyName, const Vector2& CellNumber, const Vector4& Data);

	void WriteData(const string& KeyName, size_t CellX, size_t CellY, const int& Data);
	void WriteData(const string& KeyName, size_t CellX, size_t CellY, const string& Data);
	void WriteData(const string& KeyName, size_t CellX, size_t CellY, const wstring& Data);
	void WriteData(const string& KeyName, size_t CellX, size_t CellY, const float& Data);
	void WriteData(const string& KeyName, size_t CellX, size_t CellY, const double& Data);
	void WriteData(const string& KeyName, size_t CellX, size_t CellY, const bool& Data);
	void WriteData(const string& KeyName, size_t CellX, size_t CellY, const Vector2& Data);
	void WriteData(const string& KeyName, size_t CellX, size_t CellY, const Vector3& Data);
	void WriteData(const string& KeyName, size_t CellX, size_t CellY, const Vector4& Data);

	int ReadIntData(const string& KeyName, size_t CellX, size_t CellY, int SheetIndex = 0);
	string ReadStringData(const string& KeyName, size_t CellX, size_t CellY, int SheetIndex = 0);
	wstring ReadWStringData(const string& KeyName, size_t CellX, size_t CellY, int SheetIndex = 0);
	float ReadFloatData(const string& KeyName, size_t CellX, size_t CellY, int SheetIndex = 0);
	double ReadDoubleData(const string& KeyName, size_t CellX, size_t CellY, int SheetIndex = 0);
	bool ReadBoolData(const string& KeyName, size_t CellX, size_t CellY, int SheetIndex = 0);
	Vector2 ReadVector2Data(const string& KeyName, size_t CellX, size_t CellY, int SheetIndex = 0);
	Vector3 ReadVector3Data(const string& KeyName, size_t CellX, size_t CellY, int SheetIndex = 0);
	Vector4 ReadVector4Data(const string& KeyName, size_t CellX, size_t CellY, int SheetIndex = 0);

	void ReadData(const string& KeyName, size_t CellX, size_t CellY, int& Data, int SheetIndex = 0);
	void ReadData(const string& KeyName, size_t CellX, size_t CellY, bool& Data, int SheetIndex = 0);
	void ReadData(const string& KeyName, size_t CellX, size_t CellY, double& Data, int SheetIndex = 0);
	void ReadData(const string& KeyName, size_t CellX, size_t CellY, float& Data, int SheetIndex = 0);
	void ReadData(const string& KeyName, size_t CellX, size_t CellY, string& Data, int SheetIndex = 0);
	void ReadData(const string& KeyName, size_t CellX, size_t CellY, wstring& Data, int SheetIndex = 0);
	void ReadData(const string& KeyName, size_t CellX, size_t CellY, Vector2& Data, int SheetIndex = 0);
	void ReadData(const string& KeyName, size_t CellX, size_t CellY, Vector3& Data, int SheetIndex = 0);
	void ReadData(const string& KeyName, size_t CellX, size_t CellY, Vector4& Data, int SheetIndex = 0);

	void AddSheet(const string& KeyName, const string& SheetName);

private:
	JEONG::ExcelData* CreateExcel(const string& KeyName);
	bool SaveExcel(const string& KeyName, const string& PathKey = DATA_PATH);
	bool LoadExcel(const string& KeyName, const string& PathKey = DATA_PATH);

	JEONG::ExcelData* FindExcelData(const string& KeyName);

private:
	unordered_map<string, ExcelData*> m_ExcelDataMap;

public:
	CLASS_IN_SINGLE(ExcelManager)
};

JEONG_END


