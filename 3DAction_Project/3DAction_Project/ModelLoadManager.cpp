
// ============================================================
// ヘッダー
// ============================================================
// 必須ヘッダー
#include "ModelLoadManager.h"



// ============================================================
// モデルハンドルを追加
// ============================================================
Handle ModelLoadManager::AddModelHandle(const String& _name, 
	const ModelHandle& _modelHandle)
{
	// モデルハンドルを追加
	return m_ModelLoadManager.AddData((Hashed_String)_name, _modelHandle);
}


// ============================================================
// モデルハンドルを取得
// ============================================================
const ModelHandle* ModelLoadManager::GetModelHandle(const Handle& _handle)
{
	return m_ModelLoadManager.GetData(_handle);
}

