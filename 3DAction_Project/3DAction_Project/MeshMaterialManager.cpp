
// ==================================================================
// ヘッダー
// ==================================================================
// 必須ヘッダー
#include "MeshMaterialManager.h"


// ==================================================================
// メッシュマテリアルデータを追加
// ==================================================================
Handle MeshMaterialManager::AddMeshMaterialData(const char* _name, const std::array<Color, 3>& _color)
{
	// メッシュマテリアルデータを追加
	return m_MeshMaterialManager.AddData(_name, _color);
}


// ==================================================================
// メッシュマテリアルデータを取得
// ==================================================================
std::array<Color, 3>* MeshMaterialManager::GetMeshMaterialData(const Handle& _handle)
{
	return m_MeshMaterialManager.GetData(_handle);
}
