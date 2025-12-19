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
	const char* m_VSBufferName = "UI_VSBuffer";

	// 正射影行列バッファ名
	const char* m_CSBufferName = "UIOrthoMatrix";

	// シェーダー名
	const char* m_VSShaderName = "VS_UI";
	const char* m_PSShaderName = "PS_Texture";

public:
	// コンストラクタ・デストラクタ
	UIDrawModule() = default;
	~UIDrawModule() = default;
	
	// 初期化
	void Init(BaseDrawManager& _draw, const char* _textureName);

	// テクスチャ読み込み
	bool LoadTexture(const char* _textureName);

	// 描画
	void Draw(const char* _textureName);
};

