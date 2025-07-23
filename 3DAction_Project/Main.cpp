#include "Main.h"

// メモリーリーク検出用
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

int main(void)
{
#if defined(DEBUG) || defined(_DEBUG)
	// デバッグ時にメモリリークを発見できるようにする
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	// ウィンドウズプラットフォームを作成
	PlatformWindowsSystem system{ SCREEN_WIDTH,SCREEN_HEIGHT,WINDOW_NAME,WINDOW_CLASS_NAME };
	system.Execute(); // 起動

	return 0;
}