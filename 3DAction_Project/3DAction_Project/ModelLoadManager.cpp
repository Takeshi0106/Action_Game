
// ============================================================
// ヘッダー
// ============================================================
// 必須ヘッダー
#include "ModelLoadManager.h"



// ============================================================
// モデルハンドルを追加
// ============================================================
Handle ModelLoadManager::AddModelHandle(const char* _name, const ModelHandle& _modelHandle)
{
	// モデルハンドルを追加
	return m_ModelLoadManager.AddData(_name, _modelHandle);
}


// ============================================================
// モデルハンドルを取得
// ============================================================
const ModelHandle* ModelLoadManager::GetModelHandle(const Handle& _handle)
{
	return m_ModelLoadManager.GetData(_handle);
}
