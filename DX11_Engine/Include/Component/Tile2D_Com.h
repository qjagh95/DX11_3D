#pragma once
#include "Component_Base.h"

JEONG_BEGIN

class Shader;
class Mesh;
class TileImage_Com;
class JEONG_DLL Tile2D_Com : public Component_Base
{
public:
	bool Init() override;
	int Input(float DeltaTime) override;
	int Update(float DeltaTime) override;
	int LateUpdate(float DeltaTime) override;
	void Collision(float DeltaTime) override;
	void CollisionLateUpdate(float DeltaTime) override;
	void Render(float DeltaTime) override;
	Tile2D_Com* Clone() override;
	void AfterClone() override;
	void Save(BineryWrite& Writer) override;
	void Load(BineryRead& Reader) override;

	TILE2D_OPTION GetTileOption() const { return m_TileOption; }
	void SetTileOption(TILE2D_OPTION option) { m_TileOption = option; }
	void SetLineOn(bool Value) { m_isLine = Value; }
	void SetTileType(STAGE2D_TILE_TYPE type);
	void SetMesh(const string& KeyName);
	void SetWorldPos(const Vector3& Pos);

	void AddMainTileImage(const string& FileName);
	void AddSubTileImage(const string& FileName, size_t ImageCount);
	TileImage_Com* GetMainTileImage() const { return m_TileImage; }
	TileImage_Com* GetSubTileImage(size_t index) const { return m_vecTileImage[index]; }

private:
	TILE2D_OPTION m_TileOption;
	STAGE2D_TILE_TYPE m_TileType;
	Shader*	m_Shader;
	Mesh* m_Mesh;
	ID3D11InputLayout* m_Layout;
	bool  m_isLine;

	TileImage_Com* m_TileImage;
	vector<TileImage_Com*> m_vecTileImage;

protected:
	Tile2D_Com();
	Tile2D_Com(const Tile2D_Com& CopyData);
	~Tile2D_Com();

public:
	friend class GameObject;
};

JEONG_END