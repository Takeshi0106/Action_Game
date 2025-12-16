#pragma once

// ===============================================
// クラス概要
// モデルロードマネージャー
// ===============================================


// ===============================================
// ヘッダー
// ===============================================
// モデルハンドルヘッダー
#include "ModelHandle.h"
// テンプレートマネージャーヘッダー
#include "TemplateManager.h"


// ===============================================
// クラス
// ===============================================
class ModelLoadManager
{
private:
	// --------------------------------
	// メンバー変数
	// --------------------------------
	// モデルロードマネージャー
	TemplateManager<ModelHandle> m_ModelLoadManager;


public:
	// -------------------------------
	// コンストラクタ・デストラクタ
	// -------------------------------
	ModelLoadManager() = default;
	~ModelLoadManager() = default;


	// -------------------------------
	// モデルハンドルを追加
	// -------------------------------
	Handle AddModelHandle(const char* _name, const ModelHandle& _modelHandle);


	// -------------------------------
	// モデルハンドルを取得
	// -------------------------------
	const ModelHandle* GetModelHandle(const Handle& _handle);


	// -------------------------------
	// モデルが存在するか確認
	// -------------------------------
	bool CheckModelHandle(const char* _name) {
		return m_ModelLoadManager.Exists(_name);
	}


	// -------------------------------
	// モデルハンドルを取得
	// -------------------------------
	const Handle GetModelHandle(const char* _name) {
		const Handle handle = m_ModelLoadManager.GetHandle(_name);
		return handle;
	}
};

