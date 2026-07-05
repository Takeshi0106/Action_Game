
// ===============================
// ヘッダー
// ===============================
// 必須ヘッダー
#include "DirectX_Input.h"
// Windowsの定義ヘッダー
#include <hidusage.h>
// 配列ヘッダー
#include <vector>
// ログ出力ヘッダー
#include "ReportMessage.h"


// ===============================
// 仮想キーを定義
// ===============================
struct VK
{
	static constexpr uint16_t Up = VK_UP;
	static constexpr uint16_t Down = VK_DOWN;
	static constexpr uint16_t Left = VK_LEFT;
	static constexpr uint16_t Right = VK_RIGHT;
	static constexpr uint16_t Space = VK_SPACE;
	static constexpr uint16_t Enter = VK_RETURN;
	static constexpr uint16_t Escape = VK_ESCAPE;
	static constexpr uint16_t Shift = VK_SHIFT;

	static constexpr uint16_t W = 'W';
	static constexpr uint16_t A = 'A';
	static constexpr uint16_t S = 'S';
	static constexpr uint16_t D = 'D';
};


// ===============================
// 静的メンバ変数定義・初期化
// ===============================
// 自作 KeyCode と添え字を合わせてください。
const uint16_t DirectX_Input::kKeyCodeToDXKeyCode[KeyCode::KeyCode_Max] =
{
	VK::Up,
	VK::Down,
	VK::Left,
	VK::Right,
	VK::Space,
	VK::Enter,
	VK::Escape,
	VK::Shift,

	VK::W,
	VK::A,
	VK::S,
	VK::D
};
// キーボード
bool DirectX_Input::m_NowKey[256] = {false};
bool DirectX_Input::m_OldKey[256] = {false};

// マウスキー
bool DirectX_Input::m_NowMouse[MouseCode::Mouse_Max] = {false};
bool DirectX_Input::m_OldMouse[MouseCode::Mouse_Max] = {false};

// マウス位置
Vector2 DirectX_Input::m_NowMousePos = {};
Vector2 DirectX_Input::m_OldMousePos = {};


// ===============================
// 初期化
// ===============================
bool DirectX_Input::Init()
{
	// RawInputのデバイス情報設定
	RAWINPUTDEVICE rid[2] {};

	// キーボード
	rid[0].usUsagePage = HID_USAGE_PAGE_GENERIC;
	rid[0].usUsage = HID_USAGE_GENERIC_KEYBOARD;
	rid[0].dwFlags = 0;
	rid[0].hwndTarget = nullptr;

	// マウス
	rid[1].usUsagePage = HID_USAGE_PAGE_GENERIC;
	rid[1].usUsage = HID_USAGE_GENERIC_MOUSE;
	rid[1].dwFlags = 0;
	rid[1].hwndTarget = nullptr;

	// RawInputデバイス登録
	if (!RegisterRawInputDevices(rid, 2, sizeof(RAWINPUTDEVICE))) {
		ErrorLog::OutputToConsole(u8"RawInputデバイス登録に失敗しました");
		return false;
	}

	DebugLog::OutputToConsole(u8"RawInputデバイス登録に成功しました");
	return true;
}


// ===============================
// 更新
// ===============================
void DirectX_Input::Update()
{
	// 毎フレーム OldKey を更新
	memcpy(m_OldKey, m_NowKey, sizeof(m_NowKey));

	// 毎フレーム OldMouse を更新
	memcpy(m_OldMouse, m_NowMouse, sizeof(m_NowMouse));

	// マウス位置更新
	m_OldMousePos = m_NowMousePos;
}


// ===============================
// キーのイベント受け取り時の関数
// ===============================
void DirectX_Input::HandleRawInput(LPARAM lparam)
{
	// データサイズ
	UINT size = 0;

	// データサイズ取得
	GetRawInputData((HRAWINPUT)lparam, RID_INPUT, nullptr, &size, sizeof(RAWINPUTHEADER));
	// データサイズから配列確保
	std::vector<BYTE> buffer(size);
	// データ取得
	GetRawInputData((HRAWINPUT)lparam, RID_INPUT, buffer.data(), &size, sizeof(RAWINPUTHEADER));

	// データ解析
	RAWINPUT* raw = (RAWINPUT*)buffer.data();

	// キーボード入力情報
	if (raw->header.dwType == RIM_TYPEKEYBOARD)
	{
		// キーコード
		USHORT key = raw->data.keyboard.VKey;
		// 押されているか
		bool isPressed = !(raw->data.keyboard.Flags & RI_KEY_BREAK);
		// 状態更新
		m_NowKey[key] = isPressed;
	}
	// マウス入力情報
	else if (raw->header.dwType == RIM_TYPEMOUSE)
	{
		// マウス位置更新
		m_NowMousePos.x += (float)raw->data.mouse.lLastX;
		m_NowMousePos.y += (float)raw->data.mouse.lLastY;

		// ボタン状態更新
		// 左ボタン
		if (raw->data.mouse.usButtonFlags & RI_MOUSE_LEFT_BUTTON_DOWN) {
			m_NowMouse[Mouse_Left] = true;
		}
		else if (raw->data.mouse.usButtonFlags & RI_MOUSE_LEFT_BUTTON_UP) {
			m_NowMouse[Mouse_Left] = false;
		}

		// 右ボタン
		if (raw->data.mouse.usButtonFlags & RI_MOUSE_RIGHT_BUTTON_DOWN) {
			m_NowMouse[Mouse_Right] = true;
		}
		if (raw->data.mouse.usButtonFlags & RI_MOUSE_RIGHT_BUTTON_UP) {
			m_NowMouse[Mouse_Right] = false;
		}
	}
}


// ===============================
// キーボード
// ===============================
// 押された瞬間
bool DirectX_Input::GetKeyTrigger(KeyCode key)
{
	if (m_NowKey[kKeyCodeToDXKeyCode[key]] &&
		!m_OldKey[kKeyCodeToDXKeyCode[key]])
	{
		return true;
	}

	return false;
}

// 離れた瞬間
bool DirectX_Input::GetKeyRelease(KeyCode key)
{
	if (!m_NowKey[kKeyCodeToDXKeyCode[key]] &&
		m_OldKey[kKeyCodeToDXKeyCode[key]])
	{
		return true;
	}

	return false;
}

// 押されている間
bool DirectX_Input::GetKeyPress(KeyCode key)
{
	if (m_NowKey[kKeyCodeToDXKeyCode[key]] &&
		m_OldKey[kKeyCodeToDXKeyCode[key]])
	{
		return true;
	}

	return false;
}


// ===============================
// マウス
// ===============================
// 押した瞬間
bool DirectX_Input::GetMouseTrigger(MouseCode button)
{
	if (m_NowMouse[button] &&
		!m_OldMouse[button])
	{
		return true;
	}

	return false;
}

// 離した瞬間
bool DirectX_Input::GetMouseRelease(MouseCode button)
{
	if (!m_NowMouse[button] &&
		m_OldMouse[button])
	{
		return true;
	}

	return false;
}

// 押されている間
bool DirectX_Input::GetMousePress(MouseCode button)
{
	if (m_NowMouse[button] &&
		m_OldMouse[button])
	{
		return true;
	}

	return false;
}

// 現在のカーソル位置
Vector2 DirectX_Input::GetMousePos()
{
	return m_NowMousePos;
}

// 位置フレーム前のカーソル位置からどれくらい動いたか
Vector2 DirectX_Input::GetMouseMoveAmount()
{
	return m_NowMousePos - m_OldMousePos;
}
