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
class DebugSceneState final : public BaseSceneState
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
	// コンストラクタ・デストラクタ
	DebugSceneState() = default;
	~DebugSceneState() = default;

	// シーンの描画
	void Draw() override final;
	// シーンの終了処理
	void Uninit()  override final;
};

