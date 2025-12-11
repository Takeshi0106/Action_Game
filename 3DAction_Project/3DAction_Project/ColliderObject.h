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
// オブジェクト情報
#include "ColliderConfig.h"
// 当たり判定ヘッダー
#include "AABBCollider.h"


// ===================================
// クラス
// ===================================
class ColliderObject : public BaseObject
{
protected:
	// --------------------------------
	// メンバー変数
	// --------------------------------
	// コライダー情報
	AABBCollider m_AABBCol = {};
	// 当たったか判定
	bool m_IsHit = false;
	// オブジェクト情報
	ObjectInfo m_ObjectInfo = {};


	// --------------------------------
	// 仮想関数
	// --------------------------------
	// 派生初期化
	void DerivationInit() override = 0;

	// 当たっているときの詳細判定
	virtual void OnCollisionDetail() {}
	// 当たった時の処理
	virtual void OnCollisionStay() {}


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
	void Update() override = 0;

	// ゲッター
	const AABBCollider& GetAABBCollider() const { return m_AABBCol; }
	const bool GetIsHit() const { return m_IsHit; }
	const ObjectInfo& GetObjectInfo() const { return m_ObjectInfo; }

	// セッター
	void SetIsHit(const bool isHit) { m_IsHit = isHit; }
	void SetObjectInfo(const ObjectInfo& info) { m_ObjectInfo = info; }
};
