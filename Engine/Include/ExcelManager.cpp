#include "stdafx.h"
#include "ExcelManager.h"

JEONG_USING
SINGLETON_VAR_INIT(JEONG::ExcelManager)

JEONG::ExcelManager::ExcelManager()
{
}

JEONG::ExcelManager::~ExcelManager()
{
	unordered_map<string, ExcelData*>::iterator StartIter = m_ExcelDataMap.begin();
	unordered_map<string, ExcelData*>::iterator EndIter = m_ExcelDataMap.end();

	for (; StartIter != EndIter; StartIter++)
	{
		SAFE_DELETE(StartIter->second);
	}
}

bool JEONG::ExcelManager::Init()
{
	//Sample
	/*
	for (size_t i = 0; i < 10; i++)
	{
		WriteData("AnimationData", Vector2(0.0f, (float)i), (int)i);
	}

	for (size_t i = 0; i < 10; i++)
	{
		WriteData("Temp", Vector2(0.0f, i + 1), Vector2(i * 2, i * 5));
		WriteData("Temp", Vector2(0.0f, i + 15), Vector3(i * 2, i * 5, i * 10));
		WriteData("Temp", Vector2(0.0f, i + 35), Vector4(i * 2, i * 5, i * 20, i * 30));
	}

	vector<Vector2> a;
	vector<Vector3> b;
	vector<Vector4> c;

	for (size_t i = 0; i < 10; i++)
	{
		a.push_back(ReadVector2Data("Temp2", Vector2(0.0f, i + 1)));
		b.push_back(ReadVector3Data("Temp2", Vector2(0.0f, i + 15)));
		c.push_back(ReadVector4Data("Temp2", Vector2(0.0f, i + 35)));
	}
	*/
	return true;
}

void JEONG::ExcelManager::WriteData(const string & KeyName, const Vector2 & CellNumber, const int & Data)
{
	JEONG::ExcelData* getData = FindExcelData(KeyName);

	if (getData == NULLPTR)
		getData = CreateExcel(KeyName);

	getData->sheet->writeNum((int)(CellNumber.y + 1.0f), (int)(CellNumber.x), (double)Data);
	if (SaveExcel(KeyName) == false)
	{
		TrueAssert(true);
		return;
	}
}

void JEONG::ExcelManager::WriteData(const string & KeyName, const Vector2 & CellNumber, const string & Data)
{
	JEONG::ExcelData* getData = FindExcelData(KeyName);

	if (getData == NULLPTR)
		getData = CreateExcel(KeyName);

	wstring Temp = CA2W(Data.c_str());
	getData->sheet->writeStr((int)(CellNumber.y + 1.0f), (int)CellNumber.x, Temp.c_str());

	if (SaveExcel(KeyName) == false)
	{
		TrueAssert(true);
		return;
	}
}

void JEONG::ExcelManager::WriteData(const string & KeyName, const Vector2 & CellNumber, const wstring & Data)
{
	JEONG::ExcelData* getData = FindExcelData(KeyName);

	if (getData == NULLPTR)
		getData = CreateExcel(KeyName);

	getData->sheet->writeStr((int)(CellNumber.y + 1.0f), (int)CellNumber.x, Data.c_str());

	if (SaveExcel(KeyName) == false)
	{
		TrueAssert(true);
		return;
	}
}

void JEONG::ExcelManager::WriteData(const string & KeyName, const Vector2 & CellNumber, const float & Data)
{
	JEONG::ExcelData* getData = FindExcelData(KeyName);

	if (getData == NULLPTR)
		getData = CreateExcel(KeyName);

	getData->sheet->writeNum((int)(CellNumber.y + 1.0f), (int)(CellNumber.x), (float)Data);

	if (SaveExcel(KeyName) == false)
	{
		TrueAssert(true);
		return;
	}
}

void JEONG::ExcelManager::WriteData(const string & KeyName, const Vector2 & CellNumber, const double & Data)
{
	JEONG::ExcelData* getData = FindExcelData(KeyName);

	if (getData == NULLPTR)
		getData = CreateExcel(KeyName);

	getData->sheet->writeNum((int)(CellNumber.y + 1.0f), (int)(CellNumber.x), Data);

	if (SaveExcel(KeyName) == false)
	{
		TrueAssert(true);
		return;
	}
}

void JEONG::ExcelManager::WriteData(const string & KeyName, const Vector2 & CellNumber, const bool & Data)
{
	JEONG::ExcelData* getData = FindExcelData(KeyName);

	if (getData == NULLPTR)
		getData = CreateExcel(KeyName);

	getData->sheet->writeBool((int)(CellNumber.y + 1.0f), (int)CellNumber.x, Data);

	if (SaveExcel(KeyName) == false)
	{
		TrueAssert(true);
		return;
	}
}

void JEONG::ExcelManager::WriteData(const string & KeyName, const Vector2 & CellNumber, const Vector2 & Data)
{
	JEONG::ExcelData* getData = FindExcelData(KeyName);

	if (getData == NULLPTR)
		getData = CreateExcel(KeyName);

	getData->sheet->writeNum((int)(CellNumber.y + 1.0f), (int)CellNumber.x, (double)Data.x);
	getData->sheet->writeNum((int)(CellNumber.y + 1.0f), (int)(CellNumber.x + 1.0f), (double)Data.y);

	if (SaveExcel(KeyName) == false)
	{
		TrueAssert(true);
		return;
	}
}

void JEONG::ExcelManager::WriteData(const string & KeyName, const Vector2 & CellNumber, const Vector3 & Data)
{
	JEONG::ExcelData* getData = FindExcelData(KeyName);

	if (getData == NULLPTR)
		getData = CreateExcel(KeyName);

	getData->sheet->writeNum((int)(CellNumber.y + 1.0f), (int)CellNumber.x, (double)Data.x);
	getData->sheet->writeNum((int)(CellNumber.y + 1.0f), (int)(CellNumber.x + 1.0f), (double)Data.y);
	getData->sheet->writeNum((int)(CellNumber.y + 1.0f), (int)(CellNumber.x + 2.0f), (double)Data.z);

	if (SaveExcel(KeyName) == false)
	{
		TrueAssert(true);
		return;
	}
}

void JEONG::ExcelManager::WriteData(const string & KeyName, const Vector2 & CellNumber, const Vector4 & Data)
{
	JEONG::ExcelData* getData = FindExcelData(KeyName);

	if (getData == NULLPTR)
		getData = CreateExcel(KeyName);

	getData->sheet->writeNum((int)(CellNumber.y + 1.0f), (int)CellNumber.x, (double)Data.x);
	getData->sheet->writeNum((int)(CellNumber.y + 1.0f), (int)(CellNumber.x + 1.0f), (double)Data.y);
	getData->sheet->writeNum((int)(CellNumber.y + 1.0f), (int)(CellNumber.x + 2.0f), (double)Data.z);
	getData->sheet->writeNum((int)(CellNumber.y + 1.0f), (int)(CellNumber.x + 3.0f), (double)Data.w);

	if (SaveExcel(KeyName) == false)
	{
		TrueAssert(true);
		return;
	}
}

void JEONG::ExcelManager::WriteData(const string & KeyName, size_t CellX, size_t CellY, const int & Data)
{
	WriteData(KeyName, Vector2((float)CellX, (float)CellY), Data);
}

void JEONG::ExcelManager::WriteData(const string & KeyName, size_t CellX, size_t CellY, const string & Data)
{
	WriteData(KeyName, Vector2((float)CellX, (float)CellY), Data);
}

void JEONG::ExcelManager::WriteData(const string & KeyName, size_t CellX, size_t CellY, const wstring & Data)
{
	WriteData(KeyName, Vector2((float)CellX, (float)CellY), Data);
}

void JEONG::ExcelManager::WriteData(const string & KeyName, size_t CellX, size_t CellY, const float & Data)
{
	WriteData(KeyName, Vector2((float)CellX, (float)CellY), Data);
}

void JEONG::ExcelManager::WriteData(const string & KeyName, size_t CellX, size_t CellY, const double & Data)
{
	WriteData(KeyName, Vector2((float)CellX, (float)CellY), Data);
}

void JEONG::ExcelManager::WriteData(const string & KeyName, size_t CellX, size_t CellY, const bool & Data)
{
	WriteData(KeyName, Vector2((float)CellX, (float)CellY), Data);
}

void JEONG::ExcelManager::WriteData(const string & KeyName, size_t CellX, size_t CellY, const Vector2 & Data)
{
	WriteData(KeyName, Vector2((float)CellX, (float)CellY), Data);
}

void JEONG::ExcelManager::WriteData(const string & KeyName, size_t CellX, size_t CellY, const Vector3 & Data)
{
	WriteData(KeyName, Vector2((float)CellX, (float)CellY), Data);
}

void JEONG::ExcelManager::WriteData(const string & KeyName, size_t CellX, size_t CellY, const Vector4 & Data)
{
	WriteData(KeyName, Vector2((float)CellX, (float)CellY), Data);
}

int JEONG::ExcelManager::ReadIntData(const string & KeyName, size_t CellX, size_t CellY, int SheetIndex)
{
	if (LoadExcel(KeyName) == false)
		return 0;

	ExcelData* getData = FindExcelData(KeyName);

	if (getData == NULLPTR)
	{
		TrueAssert(true);
		return false;
	}
	Sheet* getSheet = getData->book->getSheet(SheetIndex);
	getData->sheet = getSheet;

	return (int)getData->sheet->readNum((int)(CellY + 1), (int)CellX);
}

string JEONG::ExcelManager::ReadStringData(const string & KeyName, size_t CellX, size_t CellY, int SheetIndex)
{
	string a = "";

	if (LoadExcel(KeyName) == false)
		return a;

	ExcelData* getData = FindExcelData(KeyName);

	if (getData == NULLPTR)
	{
		TrueAssert(true);
		return a;
	}

	Sheet* getSheet = getData->book->getSheet(SheetIndex);
	getData->sheet = getSheet;

	wstring Temp = getSheet->readStr((int)(CellY + 1), ((int)CellX));
	string ReturnString = CW2A(Temp.c_str());

	return ReturnString;
}

wstring JEONG::ExcelManager::ReadWStringData(const string & KeyName, size_t CellX, size_t CellY, int SheetIndex)
{
	wstring a = L"";

	if (LoadExcel(KeyName) == false)
		return a;

	ExcelData* getData = FindExcelData(KeyName);

	if (getData == NULLPTR)
	{
		TrueAssert(true);
		return a;
	}

	Sheet* getSheet = getData->book->getSheet(SheetIndex);
	getData->sheet = getSheet;

	wstring Temp = getSheet->readStr((int)(CellY + 1), ((int)CellX));
	return Temp;
}

float JEONG::ExcelManager::ReadFloatData(const string & KeyName, size_t CellX, size_t CellY, int SheetIndex)
{
	if (LoadExcel(KeyName) == false)
		return 0;

	ExcelData* getData = FindExcelData(KeyName);

	if (getData == NULLPTR)
	{
		TrueAssert(true);
		return -1.0f;
	}
	Sheet* getSheet = getData->book->getSheet(SheetIndex);
	getData->sheet = getSheet;

	return (float)getSheet->readNum((int)CellY + 1, (int)CellX);
}

double JEONG::ExcelManager::ReadDoubleData(const string & KeyName, size_t CellX, size_t CellY, int SheetIndex)
{
	if (LoadExcel(KeyName) == false)
		return -1.0f;

	ExcelData* getData = FindExcelData(KeyName);

	if (getData == NULLPTR)
	{
		TrueAssert(true);
		return -1.0f;
	}
	Sheet* getSheet = getData->book->getSheet(SheetIndex);
	getData->sheet = getSheet;

	return getSheet->readNum((int)(CellY + 1), (int)CellX);
}

bool JEONG::ExcelManager::ReadBoolData(const string & KeyName, size_t CellX, size_t CellY, int SheetIndex)
{
	if (LoadExcel(KeyName) == false)
		return false;

	ExcelData* getData = FindExcelData(KeyName);

	if (getData == NULLPTR)
	{
		TrueAssert(true);
		return false;
	}
	Sheet* getSheet = getData->book->getSheet(SheetIndex);
	getData->sheet = getSheet;

	return getSheet->readBool((int)(CellY + 1), (int)CellX);
}

Vector2 JEONG::ExcelManager::ReadVector2Data(const string & KeyName, size_t CellX, size_t CellY, int SheetIndex)
{
	if (LoadExcel(KeyName) == false)
		return Vector2();

	ExcelData* getData = FindExcelData(KeyName);

	if (getData == NULLPTR)
	{
		TrueAssert(true);
		return false;
	}

	Sheet* getSheet = getData->book->getSheet(SheetIndex);
	getData->sheet = getSheet;

	
	Vector2 ReturnValue = Vector2((float)getSheet->readNum((int)(CellY + 1), (int)CellX), (float)getSheet->readNum((int)(CellX + 1), (int)(CellY + 1)));
	return ReturnValue;
}

Vector3 JEONG::ExcelManager::ReadVector3Data(const string & KeyName, size_t CellX, size_t CellY, int SheetIndex)
{
	if (LoadExcel(KeyName) == false)
		return Vector3();

	ExcelData* getData = FindExcelData(KeyName);

	if (getData == NULLPTR)
	{
		TrueAssert(true);
		return false;
	}

	Sheet* getSheet = getData->book->getSheet(SheetIndex);
	getData->sheet = getSheet;

	Vector3 ReturnValue = Vector3((float)getSheet->readNum((int)(CellY + 1), (int)CellX) , (float)getSheet->readNum((int)(CellY + 1), (int)(CellX + 1)) , (float)getSheet->readNum((int)(CellY + 1), (int)CellX + 2));

	return ReturnValue;
}

Vector4 JEONG::ExcelManager::ReadVector4Data(const string & KeyName, size_t CellX, size_t CellY, int SheetIndex)
{
	if (LoadExcel(KeyName) == false)
		return Vector4();

	ExcelData* getData = FindExcelData(KeyName);

	if (getData == NULLPTR)
	{
		TrueAssert(true);
		return false;
	}

	Sheet* getSheet = getData->book->getSheet(SheetIndex);
	getData->sheet = getSheet;

	Vector4 ReturnValue = Vector4((float)getSheet->readNum((int)(CellY + 1), (int)CellX), (float)getSheet->readNum((int)(CellY + 1), (int)(CellX + 1)), (float)getSheet->readNum((int)(CellY + 1), (int)(CellX + 2)), (float)getSheet->readNum((int)(CellY + 1), (int)(CellX + 3)));

	return ReturnValue;
}
void JEONG::ExcelManager::ReadData(const string& KeyName, size_t CellX, size_t CellY, int& Data, int SheetIndex)
{
	Data = ReadIntData(KeyName, CellX, CellY, SheetIndex);
}
void JEONG::ExcelManager::ReadData(const string& KeyName, size_t CellX, size_t CellY, bool& Data, int SheetIndex)
{
	Data = ReadBoolData(KeyName, CellX, CellY, SheetIndex);
}

void JEONG::ExcelManager::ReadData(const string & KeyName, size_t CellX, size_t CellY, double & Data, int SheetIndex)
{
	Data = ReadDoubleData(KeyName, CellX, CellY, SheetIndex);
}

void JEONG::ExcelManager::ReadData(const string & KeyName, size_t CellX, size_t CellY, float & Data, int SheetIndex)
{
	Data = ReadFloatData(KeyName, CellX, CellY, SheetIndex);
}

void JEONG::ExcelManager::ReadData(const string & KeyName, size_t CellX, size_t CellY, string & Data, int SheetIndex)
{
	Data = ReadStringData(KeyName, CellX, CellY, SheetIndex);
}

void JEONG::ExcelManager::ReadData(const string & KeyName, size_t CellX, size_t CellY, wstring & Data, int SheetIndex)
{
	Data = ReadWStringData(KeyName, CellX, CellY, SheetIndex);
}

void JEONG::ExcelManager::ReadData(const string & KeyName, size_t CellX, size_t CellY, Vector2 & Data, int SheetIndex)
{
	Data = ReadVector2Data(KeyName, CellX, CellY, SheetIndex);
}

void JEONG::ExcelManager::ReadData(const string & KeyName, size_t CellX, size_t CellY, Vector3 & Data, int SheetIndex)
{
	Data = ReadVector3Data(KeyName, CellX, CellY, SheetIndex);
}

void JEONG::ExcelManager::ReadData(const string & KeyName, size_t CellX, size_t CellY, Vector4 & Data, int SheetIndex)
{
	Data = ReadVector4Data(KeyName, CellX, CellY, SheetIndex);
}

JEONG::ExcelData* JEONG::ExcelManager::CreateExcel(const string & KeyName)
{
	JEONG::ExcelData* newData = FindExcelData(KeyName);

	if (newData != NULLPTR)
		return newData;

	wstring Temp = CA2W(KeyName.c_str());

	newData = new JEONG::ExcelData();
	newData->book = xlCreateBookW();
	newData->sheet = newData->book->addSheet(Temp.c_str());

	m_ExcelDataMap.insert(make_pair(KeyName, newData));

	return newData;
}

void JEONG::ExcelManager::AddSheet(const string & KeyName, const string & SheetName)
{
	ExcelData* getData = FindExcelData(KeyName);
	
	if (getData == NULLPTR)
	{
		TrueAssert(true);
		return;
	}

	wstring Temp = CA2W(SheetName.c_str());
	getData->book->addSheet(Temp.c_str());
}

JEONG::ExcelData * JEONG::ExcelManager::FindExcelData(const string & KeyName)
{
	unordered_map<string, ExcelData*>::iterator FindIter = m_ExcelDataMap.find(KeyName);

	if (FindIter == m_ExcelDataMap.end())
		return NULLPTR;

	return FindIter->second;
}

bool JEONG::ExcelManager::SaveExcel(const string& KeyName, const string& PathKey)
{
	ExcelData* getData = FindExcelData(KeyName);

	if (getData == NULLPTR)
		return false;

	wstring Path = PathManager::Get()->FindPath(PathKey);

	if (Path.empty() == true)
		return false;

	wstring Temp = CA2W(KeyName.c_str());
	Path += Temp;
	Path += L".xls";

	return getData->book->save(Path.c_str());
}

bool JEONG::ExcelManager::LoadExcel(const string& KeyName, const string& PathKey)
{
	ExcelData* getData = FindExcelData(KeyName);

	if (getData == NULLPTR)
		getData = CreateExcel(KeyName);

	wstring Path = PathManager::Get()->FindPath(PathKey);

	if (Path.empty() == true)
	{
		TrueAssert(true);
		return false;
	}

	wstring Temp = CA2W(KeyName.c_str());
	Path += Temp;
	Path += L".xls";
	
	return getData->book->load(Path.c_str());
}