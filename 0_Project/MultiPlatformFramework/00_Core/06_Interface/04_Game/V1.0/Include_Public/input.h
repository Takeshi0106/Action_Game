#pragma once

// ===============================
// 入力設定
// これを派生させて、プラットフォームごとの入力処理を実装する
// ゲーム内ではInputクラスのポインタを通して操作する
// ===============================


// ===============================
// ヘッダー
// ===============================
#include "Vector2.h"


// ===============================
// キー列挙型
// ===============================
enum KeyCode
{
	KeyCode_Up = 0,        // 上
	KeyCode_Down,          // 下
	KeyCode_Left,          // 左
	KeyCode_Right,         // 右
	KeyCode_Space,         // スペース
	KeyCode_Enter,         // エンター
	KeyCode_Escape,        // エスケープ
	KeyCode_Shift,         // シフト

	KeyCode_W,            // W
	KeyCode_A,            // A
	KeyCode_S,            // S
	KeyCode_D,            // D

	KeyCode_Max
};

// ===============================
// マウス列挙型
// ===============================
enum MouseCode
{
	Mouse_Left = 0,    // 左ボタン
	Mouse_Right,       // 右ボタン
	Mouse_Max
};


// ===============================
// 関数
// ===============================
class Input
{
public:
	Input() = default;
	virtual ~Input() = default;

	// -------- キーボード --------- //
	virtual bool GetKeyTrigger(KeyCode key) = 0; // 押した瞬間
	virtual bool GetKeyRelease(KeyCode key) = 0; // 離した瞬間
	virtual bool GetKeyPress(KeyCode key) = 0;   // 押されている間

	// --------- マウス ---------- //
	virtual bool GetMouseTrigger(MouseCode button) = 0;  // 押した瞬間
	virtual bool GetMouseRelease(MouseCode button) = 0;  // 離した瞬間
	virtual bool GetMousePress(MouseCode button) = 0;    // 押されている間
	virtual Vector2 GetMousePos() = 0;  // 現在のカーソル位置
	virtual Vector2 GetMouseMoveAmount() = 0; // マウスの移動量取得
};
