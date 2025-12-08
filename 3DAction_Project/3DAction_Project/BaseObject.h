#pragma once

// ========================
// ヘッダー
// ========================
// 描画マネージャー
#include "BaseDrawManager.h"
// コライダーマネージャー
#include "ColliderManager.h"
// 数学計ヘッダー
#include "Vector3.h"
#include "Quaternionh.h"
#include "Matrix4x4.h"
// 文字列
#include <string>


// =========================
// クラス
// =========================
class BaseObject
{
protected:
	// 描画マネージャー
	BaseDrawManager* m_Draw = nullptr;
	// コライダーマネージャー
	ColliderManager* m_Collider = nullptr;

	// このオブジェクトが更新する定数バッファ名
	const std::string m_TransformCBName = "Transform";

	// 当たり判定譲歩
	ColliderInfo m_AABBColliderInfo;

	// SRT情報
	Vector3 m_Position = { 0.0f,0.0f,0.0f };
	Quaternion m_Rotation = Quaternion::CreateQuaternionFromAxisAngle({ 0,1,0 }, 0);
	Vector3 m_Scale{ 1,1,1 };
	Matrix4x4 m_WorldMatrix = Matrix4x4::CreateIdentityMatrix();


	// 遅延初期化
	virtual void LateInit() = 0;
	void CreateWorldMatrix() {
		// ワールド行列計算
		m_WorldMatrix =
			Matrix4x4::CreateScalingMatrix_LH(m_Scale) *
			Matrix4x4::CreateRotationQuaternion_LH(m_Rotation) *
			Matrix4x4::CreateTranslationMatrix_LH(m_Position);
	}

public:
	// コンストラクタ・デストラクタ
	BaseObject() = default;
	virtual ~BaseObject() = default;

	// 基本関数
	void Init(BaseDrawManager* _drawManager,
		ColliderManager* _colliderManager)
	{
		// 保存
		m_Draw = _drawManager;
		m_Collider = _colliderManager;

		// 派生初期化呼び出し
		LateInit();
	}

	// 描画
	virtual void Draw() = 0;
	virtual void Update() = 0;
	virtual void Uninit() = 0;

	// ゲッター
	const ColliderInfo& GetAABBColliderInfo() const { return m_AABBColliderInfo; }
	// SRT
	const Vector3& GetPosition() const { return m_Position; }
	const Quaternion& GetRotation() const { return m_Rotation; }
	const Vector3& GetScale() const { return m_Scale; }

	// セッター
	void SetPosition(const Vector3& pos) { m_Position = pos; }
	void SetRotation(const Quaternion& rot) { m_Rotation = rot; }
	void SetScale(const Vector3& scale) { m_Scale = scale; }

};

