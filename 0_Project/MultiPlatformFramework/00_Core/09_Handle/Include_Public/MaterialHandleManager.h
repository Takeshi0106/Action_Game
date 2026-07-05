#pragma once

// ==========================================
// 【クラス概要】
// マテリアルハンドルマネージャー
// マテリアルのハンドルを管理するマネージャー
// ==========================================


// ==========================================
// ヘッダー
// ==========================================
// マテリアルハンドルヘッダー
#include "MaterialHandle.h"
// テンプレートマネージャーヘッダー
#include "TemplateManager.h"


// =========================================
// クラス
// =========================================
class MaterialHandleManager final
{
private:
	// --------------------------------
	// メンバー変数
	// --------------------------------
	TemplateManager<MaterialHandle> m_MaterialHandles;


public:
	// --------------------------------
	// コンストラクタ・デストラクタ
	// --------------------------------
	MaterialHandleManager() = default;
	~MaterialHandleManager() = default;

	// --------------------------------
	// マテリアルハンドル管理
	// --------------------------------
	Handle AddMaterialHandle(
		const Hashed_String& name,
		const MaterialHandle& materialHandle) {
		return m_MaterialHandles.AddData(name, materialHandle);
	}

	// --------------------------------
	// マテリアルハンドル取得
	// --------------------------------
	MaterialHandle* GetMaterialHandle(const Handle& handle) {
		return m_MaterialHandles.GetData(handle);
	}

	// --------------------------------
	// マテリアルハンドル存在確認
	// --------------------------------
	bool ExistsMaterialHandle(const Hashed_String& name) {
		return m_MaterialHandles.Exists(name);
	}

	// --------------------------------
	// マテリアルハンドル取得（名前から）
	// --------------------------------
	Handle GetMaterialHandleByName(const Hashed_String& name) {
		return m_MaterialHandles.GetHandle(name);
	}

	// --------------------------------
	// マテリアルハンドル削除
	// --------------------------------
	void ReleaseMaterialHandle(const Handle& handle) {
		m_MaterialHandles.Remove(handle);
	}

	// --------------------------------
	// マテリアルハンドル全削除
	// --------------------------------
	void ReleaseAllMaterialHandles() {
		m_MaterialHandles.ALLClear();
	}
};
