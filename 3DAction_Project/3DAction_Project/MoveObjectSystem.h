#pragma once

// ===================================
// 【クラス概要】
// オブジェクト移動システムクラス
// ===================================


// ===================================
// ヘッダー
// ===================================
#include "ObjectSystem.h"
#include <cstdint>
#include <string>
#include "AABBTree.h"


// ===================================
// クラス
// ===================================
class MoveObjectSystem final : public ObjectSystem
{
private:
	// --------------------------------
	// 共通変数
	// --------------------------------
	const float kMoveVelocity = 0.1f;
	const uint16_t kObjectCount = 100;

	// 大きさ
	const Vector3 kObjectSize = { 1.0f, 1.0f, 1.0f };

	// --------------------------------
	// メンバー変数
	// --------------------------------
	// 描画マネージャー
	BaseDrawManager* m_Draw = nullptr;
	// モデル名
	std::string m_ModelName = "Mech_FinnTheFrog";

	// シェーダー名
	std::string m_VSName = "VS_Object";
	std::string m_PSName = "PS_TextureModel";

	// 定数バッファ
	const std::string m_TransformCBName = "Transform";

	// --------------------------------
	// メンバー配列変数
	// --------------------------------
	std::vector<Vector3> m_Positions;
	// AABBコライダー配列
	std::vector<AABBCollider> m_FatAABBColliders;
	std::vector<AABBCollider> m_AABBColliders;

	// 向かう位置
	Vector3 m_TargetPos = { 0.0f, 0.0f, 0.0f };

public:
	// --------------------------------
	// コンストラクタ・デストラクタ
	// --------------------------------
	MoveObjectSystem() = default;
	~MoveObjectSystem() override final = default;

	// --------------------------------
	// メンバー関数
	// --------------------------------
	// 通常関数
	bool Init(BaseDrawManager* _draw) override final;
	void Update(float deltaTime) override final;
	void Draw() override final;
	void Uninit() override final;

	// 当たり判定処理
	void OnCollision() override final;

	// セッター
	void SetTargetPos(const Vector3& targetPos) { m_TargetPos = targetPos; }

	// ゲッター
	std::vector<AABBCollider>& GetAABBColliders() { return m_AABBColliders; }
	std::vector<AABBCollider>& GetFatAABBColliders() { return m_FatAABBColliders; }
};

