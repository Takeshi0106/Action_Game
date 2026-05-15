

// =========================================================
// ヘッダー
// =========================================================
// エラーメッセージ出力用
#include "ReportMessage.h"
// アクションゲームを作成
#include "ActionGame.h"
// スマートポインタ
#include<memory>
// パスの設定コンフィグ
#include "DrawPathConfig.h"

#if defined(DEBUG) || defined(_DEBUG)
// メモリーリーク検出用
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

#endif


// =========================================================
// Windows プラットフォームの場合の本編
// DirectX11を使用します
// =========================================================
#if defined(_WIN32)


// =========================================================
// Windowsプラットフォーム用ヘッダー
// =========================================================
// プラットフォームのヘッダー
#include "PlatformWindowsSystem.h"
// 固定長整数
#include <cstdint>


// =========================================================
// ウィンドウの初期化用変数
// =========================================================
namespace {
	constexpr uint16_t   SCREEN_WIDTH        = 1280;
	constexpr uint16_t   SCREEN_HEIGHT       = 720;
	constexpr wchar_t        WINDOW_NAME[]       = L"GameWindow";
	constexpr wchar_t        WINDOW_CLASS_NAME[] = L"ゲーム";

	const DrawPathConfig PATH_CONFIG= {
		u8"Asset/Shader/DX11/Compile",
		u8"Asset/Shader/Hlsl",

		u8"Asset/Info/ShaderReflection.txt",

		u8"Asset/Texture",
		u8"Asset/ObjModel"
	};

#if defined(DEBUG) || defined(_DEBUG)
	// 開発用のファイルパス
	const DevelopmentPath DEV_PATH_Config = {
		u8"Asset/Shader/SIRV",
		u8"Asset/Shader/Reflection"
	};

	// Dxc.exeのパス
	const String DXC_Path = u8"C:/0_Spirv/dxc/dxc.exe";
# endif

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


#if defined(DEBUG) || defined(_DEBUG)
	// ウィンドウズプラットフォームを作成
	PlatformWindowsSystem system(SCREEN_WIDTH, SCREEN_HEIGHT,
		WINDOW_NAME, WINDOW_CLASS_NAME,
		PATH_CONFIG,
		DEV_PATH_Config,
		DXC_Path);
#else
	// ウィンドウズプラットフォームを作成
	PlatformWindowsSystem system(SCREEN_WIDTH, SCREEN_HEIGHT,
		WINDOW_NAME, WINDOW_CLASS_NAME,
		PATH_CONFIG);
#endif

	system.Execute(actionGame.get()); // 起動

	DebugLog::OutputToConsole(u8"正常に終了しました");


	return 0;
}


// =========================================================
// Android プラットフォームの場合の本編
// =========================================================
#elif defined(__ANDROID__)


// =========================================================
// Androidプラットフォーム用ヘッダー
// =========================================================
#include <android/log.h>	

void android_main(struct android_app* state)
{
	__android_log_print(ANDROID_LOG_INFO, "MyGame", "Game started");
}


#endif

