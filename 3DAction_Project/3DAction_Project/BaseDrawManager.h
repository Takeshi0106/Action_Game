#pragma once

// ===========================================================
// 【クラス概要】
// 描画マネージャーの基底クラス
// このクラスを派生させてプラットフォーム別の
// 描画マネージャーを作成する
// 
// 描画データは型削除 (type erasure) で渡す
// デバッグ時はサイズを比較してエラーが出せるようにする
// ===========================================================



// ==============================
// ヘッダー
// ==============================
#include "BufferSetting.h" // 設定用ヘッダー

// ==============================
// クラス
// ==============================
class BaseDrawManager
{
private:

public:

	// 描画
	virtual void Draw(const char* id, const void* data, const int size) = 0;

	// 頂点バッファ作成
	virtual void CreateVertexBuffer(
		const char* drawID, 
		const void* data, 
		int size,
		PrimitiveType type = PrimitiveType::TriangleStrip,
		BufferUsage usage = BufferUsage::Dynamic,
		CPUAccess access = CPUAccess::Write) = 0;

	// バッファ更新
	// 定数バッファ更新
	virtual void UpdateShaderConstants(const char* id, const void* data, const int size) = 0;
	// 頂点バッファ更新
	virtual void UpdateVertexBuffer(const char* drawID, const void* data, int size) = 0;
};

