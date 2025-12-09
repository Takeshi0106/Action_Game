#pragma once

// ===================================
// 【クラス概要】
// プレイヤークラス
// ===================================


// ===================================
// ヘッダー
// ===================================
#include "BaseObject.h"
#include <string>

#if defined(DEBUG) || defined(_DEBUG)
// ＢＯＸ描画
#include "Box.h"
#endif


// ===================================
// クラス
// ===================================
class Player final : public BaseObject
{
private:
	// モデル名
	std::string m_ModelName = "Mech_FinnTheFrog";

	// シェーダー名
	std::string m_VSName = "VS_Object";
	std::string m_PSName = "PS_TextureModel";

	// プレイヤー当たり判定サイズ
	const Vector3 CENTER_OFFSET = { 0.0f,1.5f,0.0f };
	const Vector3 HITBOX_SCALE = { 3.5f, 3.0f, 2.8f };

	// BOX描画用
#if defined(DEBUG) || defined(_DEBUG)
	BOX m_DebugBox;
#endif

	// 遅延初期化
	void LateInit() override final;
	// デバッグ描画
	void DebugDrawBox();

public:
	// コンストラクタ・デストラクタ
	Player() = default;
	~Player() override final = default;

	// 描画
	void Draw() override final;
	// 更新
	void Update() override final;
	// 後処理
	void Uninit() override final;
};

