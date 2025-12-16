#pragma once

// =====================================
// クラス定義
// 描画時に使用する描画コマンドクラス
// =====================================

// =====================================
// ヘッダー
// =====================================
// ハンドル構造体
#include "Handle.h"
// 配列ヘッダー
#include <vector>
// 固定長整数ヘッダー
#include <cstdint>


// =====================================
// 構造体
// =====================================
// バッファ更新コマンド
struct UpdateBufferComand
{
	// 更新するバッファのハンドル
	Handle bufferHandle;
	// 更新するデータのポインタ
	void* dataPtr;
	// 更新するデータサイズ
	size_t dataSize;
};


// RT切り替え
struct RTChangeComand
{
	// レンダーターゲットビューハンドル配列
	std::vector<Handle> rtvHandles;
	// 深度ステンシルビューのハンドル
	Handle dsvHandle;
};

// 描画モード
enum class DrawMode : uint8_t
{
	Draw,
	DrawIndexed,
	DrawInstanced
};

// 描画コマンド
struct DrawComand
{
	// 描画モード
	DrawMode drawMode;

	// シェーダーハンドル
	Handle vsHandle;
	Handle psHandle;

	// 描画に使用するハンドル
	Handle modelHandle;

	// 更新するバッファコマンド配列
	std::vector<UpdateBufferComand> updateBufferComands;
};



