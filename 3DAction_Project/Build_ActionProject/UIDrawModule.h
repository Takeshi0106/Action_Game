#pragma once

// ================================
// UI描画用モジュール
// ================================


// ================================
// ヘッダー
// ================================
// ベース描画マネージャー
#include "BaseDrawManager.h"
// 行列
#include "Matrix4x4.h"
// 頂点構造体
#include "Vertex.h"
// 文字列ヘッダー
#include "Hashed_String.h"


// ================================
// クラス
// ================================
class UIDrawModule
{
private:
	// 描画マネージャー
	BaseDrawManager* m_Draw = nullptr;
	// 頂点情報
	static Vertex m_Vertices[4];

	// 頂点バッファ名
	const Hashed_String m_VSBufferName = Hashed_String(u8"UI_VSBuffer");

	// 正射影行列バッファ名
	const Hashed_String m_CSBufferName = Hashed_String(u8"UIOrthoMatrix");

	// シェーダー名
	const Hashed_String m_VSShaderName = Hashed_String(u8"VS_UI");
	const Hashed_String m_PSShaderName = Hashed_String(u8"PS_Texture");

public:
	// コンストラクタ・デストラクタ
	UIDrawModule() = default;
	~UIDrawModule() = default;
	
	// 初期化
	void Init(BaseDrawManager& _draw, const Hashed_String& _textureName);

	// テクスチャ読み込み
	bool LoadTexture(const Hashed_String& _textureName);

	// 描画
	void Draw(const Hashed_String& _textureName);
};

