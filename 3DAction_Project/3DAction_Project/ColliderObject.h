#pragma once

// ===================================
// 【クラス概要】
// コライダーオブジェクト基底クラス
// ===================================


// ===================================
// ヘッダー
// ===================================
// 基底オブジェクト
#include "BaseObject.h"
// 当たり判定ヘッダー
#include "AABBCollider.h"


// ===================================
// クラス
// ===================================
class ColliderObject : public BaseObject
{
private:
	// --------------------------------
	// メンバー変数
	// --------------------------------
	// コライダー情報
	AABBCollider m_AABBCol = {};

protected:
	// --------------------------------
	// 仮想関数
	// --------------------------------
	// 派生初期化
	void DerivationInit() override = 0;
	// 派生更新
	virtual void DerivationUpdate() = 0;

	// 当たっているときの詳細判定
	virtual void OnCollisionDetail() {}
	// 当たった時の処理
	virtual void OnCollisionStay() {}


	// --------------------------------
	// 純粋仮想関数
	// --------------------------------
	// AABBコライダー設定
	void AABBCollisonSetting(const ColliderPresetConfig& config)
	{
		m_AABBCol.config = config;
	}

public:
	// --------------------------------
	// コンストラクタ・デストラクタ
	// --------------------------------
	ColliderObject() = default;
	~ColliderObject() override = default;


	// --------------------------------
	// 仮想関数
	// --------------------------------
	// 描画
	virtual void Draw() override = 0;
	// 後処理
	virtual void Uninit() override = 0;


	// --------------------------------
	// 非仮想関数
	// --------------------------------
	// 更新
	void Update() override final
	{
		// 派生更新
		DerivationUpdate();

		// AABBコライダーを毎フレーム更新
		m_AABBCol = m_AABBCol.UpdateAABB(m_SRT);
	}

	// コライダー取得
	const AABBCollider& GetAABBCollider() const { return m_AABBCol; }
};
