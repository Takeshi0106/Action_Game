#pragma once

// ==========================================
// 【クラス概要】
// Windowsプラットフォーム用の初期化・終了処理を行うクラスです。
// ウィンドウを作成、初期化などを主に管理
// ゲーム本体（GameMain）に、描画マネージャー（DirectX）や音声マネージャーを渡します。
// ==========================================


// ==========================================
// ヘッダー
// ==========================================
// 基底クラスのヘッダー
#include "IPlatformSystem.h"
#include <Windows.h>
// 描画パス設定ヘッダー
#include "DrawPathConfig.h"
// 動的確保
#include <memory>
// 固定長整数ヘッダー
#include <cstdint>

// =====================================================
// 前方宣言　Windows.hを.cpp内だけでインクルードする
// =====================================================
// アプリケーションハンドルの前方宣言
struct APPLICATIONHANDLE; // HINSTANCEのラップ構造体
// DirectX用描画マネージャー
class DirectX_DrawManager;
// 入力情報受け取りクラス
class Windows_Input;
// カーソル制御クラス
class Windows_CursorController;


// =====================================================
// ウィンドウプラットフォームの初期化・後処理クラス
// =====================================================
class PlatformWindowsSystem : public IPlatformSystem
{
private:
	static APPLICATIONHANDLE m_AppInstance;      // アプリケーションのハンドル
	HWND m_WinInstance;      // ウィンドウハンドル
	const uint16_t      m_Width;	         // ウィンドウの画面横幅 
	const uint16_t      m_Height;	         // ウィンドウの画面縦幅
	const wchar_t*          m_WindowName;        // ウィンドウの名前
	const wchar_t*          m_WindowClassName;   // ウィンドウのクラス名
	const DrawPathConfig m_PathConfig; // パス設定

	static std::unique_ptr<Windows_Input> m_Input; // 入力情報
	static std::unique_ptr<Windows_CursorController> m_CursorController; // カーソル制御クラス

	// オーバーライド関数
	bool Init()       override; // 初期化処理
	void GameLoop()   override; // ゲームループ
	void Uninit()     override; // 後処理
	bool GameInit()   override; // ゲームの開始処理
	bool GameMain()   override; // ゲームのメイン処理
	void GameUninit() override; // ゲームの後処理


	// Imgui用関数　＊リリース時は何も実行しません
	void InitImGui();
	void UpdateImGui();
	void DrawImGui();
	void UninitImGui();


public:
#if defined(DEBUG) || defined(_DEBUG)
	// コンストラクタ・デストラクタ
	PlatformWindowsSystem(uint16_t _width, uint16_t _height, 
		const wchar_t* _windowClassName, const wchar_t* _windowName, 
		const DrawPathConfig& _config,
		const DevelopmentPath& _devConfig,
		const String& _dxcPath);
#else
	// コンストラクタ・デストラクタ
	PlatformWindowsSystem(uint16_t _width, uint16_t _height, 
		const wchar_t* _windowClassName, const wchar_t* _windowName, 
		const DrawPathConfig& _config);
#endif
	~PlatformWindowsSystem();

	// ゲッター
	uint16_t GetWidthSize() const  { return m_Width; }   // ウィンドウの横幅を戻り値で返す
	uint16_t GetHeightSize() const { return m_Height; }	 // ウィンドウの縦幅を戻り値で返す
};
