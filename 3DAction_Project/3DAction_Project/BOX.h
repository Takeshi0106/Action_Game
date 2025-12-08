#pragma once

// ===================================
// 【クラス概要】
// BOXクラス
// ===================================


// ===================================
// ヘッダー
// ===================================
// 頂点ヘッダー
#include "Vertex.h"
#include "Color.h"
// 固定長整数型ヘッダー
#include <cstdint>
// AABBコライダーヘッダー
#include "AABBCollider.h"
// 文字列ヘッダー
#include <string>


// ===================================
// 前方宣言
// ===================================
class BaseDrawManager;


// ===================================
// クラス
// ===================================
class BOX
{
private:
	// 頂点シェーダー
	const static std::string m_VSName;
	// ピクセルシェーダー
	const static std::string m_PSName;

	// 頂点バッファ名
	const static std::string m_VBName;
	const static std::string m_IBName;

	// 定数バッファ名
	const static std::string m_TransformCBName;

	// マテリアル名
	const static std::string m_MaterialName;

	// 頂点配列
	const static Vertex m_Vertex[24];
	const static uint32_t m_Index[36];

public:
	// コンストラクタ・デストラクタ
	BOX() = default;
	~BOX() = default;

	// 初期化
	bool Init(BaseDrawManager* drawManager);

	// 描画
	static void Draw(BaseDrawManager* drawManager, const Color& color);
	// AABB描画
	static void DrawAABB(BaseDrawManager* drawManager, const AABBCollider* aabb, const Color& color);
};

