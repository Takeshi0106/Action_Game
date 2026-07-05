#pragma once


// ===============================
// ヘッダー
// ===============================
// 基底クラスヘッダー
#include "input.h"
// 整数ヘッダー
#include <cstdint>
// Windowsヘッダー
#include <Windows.h>


// ===============================
// Windows用入力設定
// ===============================
class Windows_Input : public Input
{
private:
	// 今の列挙型の値をWindowsの仮想キーコードに変換する配列
	static const uint16_t kKeyCodeToWindowsKeyCode[KeyCode::KeyCode_Max];

	// キーボード
	static bool m_NowKey[256];
	static bool m_OldKey[256];

	// マウスキー
	static bool m_NowMouse[MouseCode::Mouse_Max];
	static bool m_OldMouse[MouseCode::Mouse_Max];

	// マウス位置
	static Vector2 m_NowMousePos;
	static Vector2 m_OldMousePos;

public:
	// コンストラクタ・デストラクタ
	Windows_Input() = default;
	~Windows_Input() = default;

	// 初期化
	bool Init();
	// 更新
	void Update();
	// キーの状態更新
	static void HandleRawInput(LPARAM lparam);

	// -------- キーボード --------- //
	bool GetKeyTrigger(KeyCode key) override final; // 押された瞬間
	bool GetKeyRelease(KeyCode key) override final; // 離れた瞬間
	bool GetKeyPress(KeyCode key) override final;   // 押されている間

	// --------- マウス ---------- //
	bool GetMouseTrigger(MouseCode button) override final;  // 押した瞬間
	bool GetMouseRelease(MouseCode button) override final ;  // 離した瞬間
	bool GetMousePress(MouseCode button) override final;    // 押されている間
	Vector2 GetMousePos() override final;  // 現在のカーソル位置
	Vector2 GetMouseMoveAmount() override final; // マウスの移動量取得
};
