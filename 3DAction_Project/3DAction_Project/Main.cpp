

// =========================================================
// ヘッダー
// =========================================================
// プラットフォームのヘッダー
#include "PlatformWindowsSystem.h"
// エラーメッセージ出力用
#include "ReportMessage.h"
// アクションゲームを作成
#include "ActionGame.h"
// スマートポインタ
#include<memory>

#if defined(DEBUG) || defined(_DEBUG)
// メモリーリーク検出用
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

#endif


// =========================================================
// ウィンドウの初期化用変数
// =========================================================
namespace {
	constexpr unsigned int   SCREEN_WIDTH        = 1280;
	constexpr unsigned int   SCREEN_HEIGHT       = 720;
	constexpr wchar_t        WINDOW_NAME[]       = L"GameWindow";
	constexpr wchar_t        WINDOW_CLASS_NAME[] = L"ゲーム";
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

	// ゲームの容量確保
	std::unique_ptr<ActionGame> actionGame;
	actionGame = std::make_unique<ActionGame>();

	// ウィンドウズプラットフォームを作成
	PlatformWindowsSystem system(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_NAME, WINDOW_CLASS_NAME);
	system.Execute(actionGame.get()); // 起動

	DebugLog::OutputToConsole("正常に終了しました");

	return 0;
}