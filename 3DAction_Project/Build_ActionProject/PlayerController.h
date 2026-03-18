#pragma once

// ============================
// クラス概要
// プレイヤーコントローラー
// キーボード入力やコントローラ入力
// を共通して処理する
// ============================

// ============================
// ヘッダー
// ============================
// 入力情報ヘッダー
#include "input.h"
// ベクトルヘッダー
#include "Vector2.h"


// ============================
// クラス
// ============================
class PlayerController
{
private:
	// 入力情報
	Input* m_Input = nullptr;

public:
	// コンストラクタ・デストラクタ
	PlayerController() = default;
	~PlayerController() = default;

	// 初期化
	void Init(Input* input) {
		m_Input = input;
	}

	// 移動ベクトル取得
	Vector2 GetMoveVector();

};

