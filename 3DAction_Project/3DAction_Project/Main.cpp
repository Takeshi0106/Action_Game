

// =========================================================
// 【注意】
// マルチスレッドに対応できていません
// 使用しないように注意してください
// =========================================================


// =========================================================
// ヘッダー
// =========================================================
// プラットフォームのヘッダー
#include "PlatformWindowsSystem.h"

#if defined(DEBUG) || defined(_DEBUG)
// エラーメッセージ出力用
#include "ReportMessage.h"

// メモリーリーク検出用
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

#endif


// =========================================================
// 初期化用変数
// =========================================================
namespace {
	constexpr unsigned int   SCREEN_WIDTH        = 1280;
	constexpr unsigned int   SCREEN_HEIGHT       = 720;
	constexpr wchar_t        WINDOW_CLASS_NAME[] = L"MyWindowClass";
	constexpr wchar_t        WINDOW_NAME[]       = L"MyGameWindow";
}


// =========================================================
// 本編
// =========================================================
int main(void)
{
#if defined(DEBUG) || defined(_DEBUG)
	// デバッグ時にメモリリークを発見できるようにする
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	// ウィンドウズプラットフォームを作成
	PlatformWindowsSystem system(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_NAME, WINDOW_CLASS_NAME);
	system.Execute(); // 起動

	DebugLog::OutputToConsole("正常に終了しました");

	return 0;
}