
// ==================================================================
// ヘッダー
// ==================================================================
// 必須ヘッダー
#include "MeshMaterialManager.h"


// ==================================================================
// メッシュマテリアルデータを追加
// ==================================================================
Handle MeshMaterialManager::AddMeshMaterialData(const String& _name, const MeshMaterialColor& _color)
{
	// メッシュマテリアルデータを追加
	return m_MeshMaterialManager.AddData((Hashed_String)_name, _color);
}


// ==================================================================
// メッシュマテリアルデータを取得
// ==================================================================
MeshMaterialColor* MeshMaterialManager::GetMeshMaterialData(const Handle& _handle)
{
	return m_MeshMaterialManager.GetData(_handle);
}
