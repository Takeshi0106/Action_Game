#pragma once

// ===============================
// デバッグ用シーン
// ===============================


// ===============================
// ヘッダー
// ===============================
// 基底クラス
#include "BaseSceneState.h"
// 基本カメラ
#include "BaseCamera.h"
// 基本ライト
#include "BaseLight.h"
// 四角形描画
#include "Square2D.h"
// ナイトモデル
#include "Knight.h"
// 動的確保
#include <memory>


// ===============================
// クラス
// ===============================
class DebugScene final : public BaseSceneState
{
private:
	// カメラ
	std::unique_ptr<BaseCamera> m_Camera;
	// ライト
	std::unique_ptr<BaseLight> m_Light;
	
	// オブジェクト
	Square2D m_Square;
	Knight m_Knight;

	// シーンの初期化
	bool DerivativeInit() override final;
	// シーンの更新
	void DerivatIveUpdate(float _delta) override final;

public:
	DebugScene() = default;
	~DebugScene() = default;

	// シーンの描画
	void Draw();
	// シーンの終了処理
	void Uninit();
};

