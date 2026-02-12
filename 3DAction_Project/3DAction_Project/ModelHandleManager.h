#pragma once

// ==========================================
// 【クラス概要】
// DirectX11用モデルハンドルマネージャー
// モデルのハンドルを管理するマネージャー
// ==========================================


// ==========================================
// ヘッダー
// ==========================================
// モデルハンドルヘッダー
#include "ModelHandle.h"
// テンプレートマネージャーヘッダー
#include "TemplateManager.h"


// =========================================
// クラス
// =========================================
class DirectX11_ModelHandleManager final
{
private:
	// --------------------------------
	// メンバー変数
	// --------------------------------
	TemplateManager<ModelHandle> m_ModelHandles;


public:
	// --------------------------------
	// コンストラクタ・デストラクタ
	// --------------------------------
	DirectX11_ModelHandleManager() = default;
	~DirectX11_ModelHandleManager() = default;


	// --------------------------------
	// モデルハンドル管理
	// --------------------------------
	Handle AddModelHandle(
		const Hashed_String& name,
		const ModelHandle& modelHandle) {
		return m_ModelHandles.AddData(name, modelHandle);
	}

	// モデルハンドル取得
	ModelHandle* GetModelHandle(const Handle& handle) {
		return m_ModelHandles.GetData(handle);
	}

	// モデルハンドル存在確認
	bool ExistsModelHandle(const Hashed_String& name) {
		return m_ModelHandles.Exists(name);
	}

	// モデルハンドル取得（名前から）
	Handle GetModelHandleByName(const Hashed_String& name) {
		return m_ModelHandles.GetHandle(name);
	}

	// モデルハンドル削除
	void ReleaseModelHandle(const Handle& handle) {
		m_ModelHandles.Remove(handle);
	}

	// モデルハンドル全削除
	void ReleaseAllModelHandle() {
		m_ModelHandles.ALLClear();
	}

};
