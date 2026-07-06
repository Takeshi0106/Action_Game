#pragma once

// ===============================
// ゲームモジュール構造体
// 
// ゲーム基板に渡す情報を格納する構造体
// ===============================


// ===============================
// ヘッダー
// ===============================
// 描画
#include "IDrawManager.h"
// 入力
#include "input.h"
// カーソル
#include "CursorController.h"


// ===============================
// ゲームモジュール構造体
// ===============================
struct GameModules
{
	IDrawManager* drawManager = nullptr;	// 描画マネージャー
	Input* input = nullptr;					// 入力
	CursorController* cursor = nullptr;		// カーソルコントローラー
};
