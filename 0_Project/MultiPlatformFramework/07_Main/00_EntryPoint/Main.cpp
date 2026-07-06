

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
// DirectX11のヘッダー
#include "DirectX11_DrawManager.h"
// 固定長整数
#include <cstdint>
// filysystem
#include <filesystem>

// ==========================================================
// プロトタイプ宣言
// ==========================================================
// アセットフォルダのパスを取得する関数
String GetAssetPath();
// アセットパスからファイルパスを取得する関数
DrawPathConfig CreateDrawPathConfig(String assetPath);
#if defined(DEBUG) || defined(_DEBUG)
// デバッグ用のパスを取得する関数
DevelopmentPath CreateDevelopmentPathConfig(String assetPath);
#endif

// =========================================================
// ウィンドウの初期化用変数
// =========================================================
namespace {
	constexpr uint16_t   SCREEN_WIDTH        = 1280;
	constexpr uint16_t   SCREEN_HEIGHT       = 720;
	constexpr wchar_t    WINDOW_NAME[]       = L"GameWindow";
	constexpr wchar_t    WINDOW_CLASS_NAME[] = L"ゲーム";

	const String AssetPath = u8"Asset";

	const DrawPathConfig PATH_CONFIG= {
		u8"Shader/DX11/Compile",
		u8"Shader/Hlsl",

		u8"Info/ShaderReflection.txt",

		u8"Texture",
		u8"ObjModel"};

#if defined(DEBUG) || defined(_DEBUG)
	// 開発用のファイルパス
	const DevelopmentPath DEV_PATH_Config = {
		u8"Shader/SIRV",
		u8"Shader/Reflection"};

	// Asset
	const String ProjectAssetPath = PROJECT_ASSET_PATH;
	// Dxc.exeのパス
	const String DXC_Path = DXC_PATH;
# endif
}


// =========================================================
// 本編
// =========================================================
int main(void)
{
	ReportInit::Init();

#if defined(DEBUG) || defined(_DEBUG)
	// デバッグ時にメモリリークを発見できるようにする
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	// アセットフォルダのパスを取得
	DrawPathConfig drawPathConfig = CreateDrawPathConfig(GetAssetPath());

	// ゲームの容量確保
	std::unique_ptr<ActionGame> actionGame;
	actionGame = std::make_unique<ActionGame>();
	// DirectX11の描画マネージャーを作成
	std::unique_ptr<DirectX_DrawManager> drawManager;
	drawManager = std::make_unique<DirectX_DrawManager>(drawPathConfig);


#if defined(DEBUG) || defined(_DEBUG)
	DevelopmentPath devPathConfig = CreateDevelopmentPathConfig(GetAssetPath());

	// ウィンドウズプラットフォームを作成
	PlatformWindowsSystem system(
		SCREEN_WIDTH, SCREEN_HEIGHT,
		WINDOW_NAME, WINDOW_CLASS_NAME,
		drawPathConfig,
		devPathConfig,
		DXC_Path);
#else
	// ウィンドウズプラットフォームを作成
	PlatformWindowsSystem system(SCREEN_WIDTH, SCREEN_HEIGHT,
		WINDOW_NAME, WINDOW_CLASS_NAME,
		drawPathConfig);
#endif

	system.Execute(actionGame.get(),drawManager.get()); // 起動

	DebugLog::OutputToConsole(u8"正常に終了しました");


	return 0;
}


#if defined(DEBUG) || defined(_DEBUG)
// アセットパスを返す関数
String GetAssetPath()
{
	// プロジェクトのパスを取得
	std::filesystem::path assetPath = ProjectAssetPath.GetU8String();
	// アセットフォルダが存在するか確認
	if (!std::filesystem::exists(assetPath)) {
		WarningLog::OutputToConsole(u8"アセットフォルダが存在しません: " + assetPath.u8string());
		return AssetPath;
	}
	// プロジェクトのアセットフォルダのパスを返す
	DebugLog::OutputToConsole(u8"アセットフォルダのパス: " + assetPath.u8string());
	return assetPath.u8string();
}
// デバッグ用のパスを作成する関数
DevelopmentPath CreateDevelopmentPathConfig(String assetPath)
{
	// 開発用のパスを作成
	std::filesystem::path basePath = std::filesystem::path(assetPath.GetU8String());
	std::filesystem::path shaderSpirvPath = basePath / DEV_PATH_Config.kSPIRVFolderPath.GetU8String();
	std::filesystem::path shaderReflectionPath = basePath / DEV_PATH_Config.kSPIRVReflectionInfoFolderPath.GetU8String();
	// 開発用のパスを設定
	DevelopmentPath devPath = {
		shaderSpirvPath.make_preferred().u8string(),
		shaderReflectionPath.make_preferred().u8string() };
	return devPath;
}
#else
String GetAssetPath()
{
	return AssetPath;
}
#endif
// アセットパスからファイルパスを作成する関数
DrawPathConfig CreateDrawPathConfig(String assetPath)
{
	// アセットパスを作成
	std::filesystem::path basePath = std::filesystem::path(assetPath.GetU8String());
	std::filesystem::path shaderBinaryPath = basePath / PATH_CONFIG.shaderBinaryPath.GetU8String();
	std::filesystem::path shaderSourcePath = basePath / PATH_CONFIG.shaderSourcePath.GetU8String();
	std::filesystem::path shaderReflectionPath = basePath / PATH_CONFIG.shaderReflectionPath.GetU8String();
	std::filesystem::path texturePath = basePath / PATH_CONFIG.texturePath.GetU8String();
	std::filesystem::path objModelPath = basePath / PATH_CONFIG.objModelPath.GetU8String();
	// DrawPathConfigを設定
	DrawPathConfig pathConfig = {
		shaderBinaryPath.make_preferred().u8string(),
		shaderSourcePath.make_preferred().u8string(),
		shaderReflectionPath.make_preferred().u8string(),
		texturePath.make_preferred().u8string(),
		objModelPath.make_preferred().u8string() };
	return pathConfig;
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

