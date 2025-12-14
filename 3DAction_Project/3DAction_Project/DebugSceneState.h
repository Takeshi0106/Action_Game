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
// 移動オブジェクトシステム
#include "MoveObjectSystem.h"
// 当たり判定チェック
#include "AABBTree.h"
#include "CollisionSystemModule.h"
// 動的確保
#include <memory>


// ===============================
// クラス
// ===============================
class DebugSceneState final : public BaseSceneState
{
private:
	// --------------------------------
	// メンバー変数
	// --------------------------------
	// 当たり判定チェックモジュール
	CollisionSystemModule m_CollisionSystem;

	// カメラ
	std::unique_ptr<BaseCamera> m_Camera;
	// ライト
	std::unique_ptr<BaseLight> m_Light;

	MoveObjectSystem m_MoveObjectSystem;

	// システム
	AABBTree m_AABBTree;

	// --------------------------------
	// メンバー関数
	// --------------------------------
	// シーンの初期化
	bool DerivativeInit() override final;
	// シーンの更新
	void DerivatIveUpdate(float _delta) override final;
	// コライダー更新チェック
	void UpdateColliderCheck();
	// 当たり判定更新
	void UpdateCollision();


	// --------------------------------
	// デバッグ関数
	// --------------------------------
	// デバッグ初期化
	void DebugInit();
	// デバッグ更新
	void DebugUpdate();
	// デバッグ描画
	void DebugDraw();

public:
	// --------------------------------
	// コンストラクタ・デストラクタ
	// --------------------------------
	DebugSceneState() = default;
	~DebugSceneState() = default;

	// --------------------------------
	// 関数
	// --------------------------------
	// シーンの描画
	void Draw() override final;
	// シーンの終了処理
	void Uninit()  override final;
};
