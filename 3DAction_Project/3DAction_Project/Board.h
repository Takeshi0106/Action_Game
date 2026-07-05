#pragma once

// ===================================
// 【クラス概要】
// ボードを描画するクラス
// ===================================


// ===================================
// ヘッダー
// ===================================
#include "Vertex.h"
#include "Color.h"
#include "Hashed_String.h"


// ===================================
// 前方宣言
// ===================================
class BaseDrawManager;


// ===================================
// クラス
// ===================================
class Board
{
private:
	// -----------------------------
	// メンバー変数
	// -----------------------------
	// 頂点シェーダー
	const static Hashed_String m_VSName;
	// ピクセルシェーダー
	const static Hashed_String m_PSName;

	// 頂点バッファ名
	const static Hashed_String m_VBName;
	const static Hashed_String m_IBName;

	// 定数バッファ名
	const static Hashed_String m_TransformCBName;

	// マテリアル名
	const static Hashed_String m_MaterialName;

	// 頂点配列
	const static Vertex m_Vertex[4];

public:
	// コンストラクタ・デストラクタ
	Board() = default;
	~Board() = default;

	// -----------------------------
	// メンバー関数
	// -----------------------------
	// 初期化
	static bool Init(BaseDrawManager* _drawManager);
	// 描画
	static void Draw(BaseDrawManager* _drawManager, const Color& _color);
};

