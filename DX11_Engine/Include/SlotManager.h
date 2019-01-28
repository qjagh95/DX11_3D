#pragma once

JEONG_BEGIN

class IconSlot_Com;
class UICon_Com;
class JEONG_DLL SlotManager
{

public:
	bool Init();
	void ChangeSlot(const string& GroupName, size_t Src, size_t Dest);
	bool CreateSlot(const string& GroupName, size_t RowCount, size_t ColumCount);

private:
	vector<IconSlot_Com*>* FindSlot(const string& GroupName);

private:
	unordered_map<string, vector<IconSlot_Com*>> m_SlotMap;

public:
	CLASS_IN_SINGLE(SlotManager)
};

JEONG_END