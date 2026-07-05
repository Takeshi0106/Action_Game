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
#include "Hashed_String.h"
#include "AABBTree.h"
#include "FatAABBCollider.h"

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
	const float kRotateSpeed = 3.0f;

	const uint16_t kObjectCount = 200;

	// 大きさ
	const Vector3 kObjectSize = { 1.0f, 1.0f, 1.0f };

	// --------------------------------
	// メンバー変数
	// --------------------------------
	// 描画マネージャー
	IDrawManager* m_Draw = nullptr;
	// モデル名
	Hashed_String m_ModelName = Hashed_String(u8"Mech_FinnTheFrog");

	// シェーダー名
	Hashed_String m_VSName = Hashed_String(u8"VS_Object");
	Hashed_String m_PSName = Hashed_String(u8"PS_TextureModel");

	// 定数バッファ
	const Hashed_String m_TransformCBName = Hashed_String(u8"Transform");

	// --------------------------------
	// メンバー配列変数
	// --------------------------------
	std::vector<Vector3> m_Positions;
	std::vector<Quaternion> m_Rotations;
	// AABBコライダー配列
	std::vector<FatAABBCollider> m_FatAABBColliders;
	std::vector<AABBCollider> m_AABBColliders;
	// 当たっているオブジェクトのインデックス配列
	std::vector<AABBTreeHandle> m_AABBHandle;

	// 補正値配列
	std::vector<Vector3> m_Correction;

	// 向かう位置
	Vector3 m_TargetPos = { 0.0f, -10.0f, 10.0f };

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
	bool Init(IDrawManager* _draw) override final;
	void Update(float deltaTime) override final;
	void Draw() override final;
	void Uninit() override final;

	// 当たり判定処理
	Vector3 ComputeMTV(uint32_t _selfInd, uint32_t _partnerInd);
	// 押し戻し更新
	void UpdateCorrection();

	// セッター
	void SetTargetPos(const Vector3& targetPos) { m_TargetPos = targetPos; }

	// ゲッター
	const Vector3& GetTargetPos() const { return m_TargetPos; }
	std::vector<AABBCollider>& GetAABBColliders() { return m_AABBColliders; }
	std::vector<FatAABBCollider>& GetFatAABBColliders() { return m_FatAABBColliders; }
	std::vector<AABBTreeHandle>& GetAABBHandle() { return m_AABBHandle; }
	std::vector<Vector3>& GetCorrection() { return m_Correction; }
};

