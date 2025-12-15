#pragma once

// ===============================================
// クラス概要
// メッシュマテリアルマネージャー
// ===============================================


// ===============================================
// ヘッダー
// ===============================================
// ハンドルヘッダー
#include "Handle.h"
// テンプレートマネージャーヘッダー
#include "TemplateManager.h"
// 色ヘッダー
#include "MeshMaterialColor.h"
// 配列ヘッダー
#include <array>


// ===============================================
// クラス
// ===============================================
class MeshMaterialManager
{
private:
	// --------------------------------
	// メンバー変数
	// --------------------------------
	// メッシュマテリアルデータマネージャー
	TemplateManager<MeshMaterialColor> m_MeshMaterialManager;


public:
	// -------------------------------
	// コンストラクタ・デストラクタ
	// -------------------------------
	MeshMaterialManager() = default;
	~MeshMaterialManager() = default;


	// -------------------------------
	// メッシュマテリアルデータを追加
	// -------------------------------
	Handle AddMeshMaterialData(const char* _name, const MeshMaterialColor& _color);


	//-------------------------------
	// メッシュマテリアルデータを取得
	// -------------------------------
	MeshMaterialColor* GetMeshMaterialData(const Handle& _handle);
};

