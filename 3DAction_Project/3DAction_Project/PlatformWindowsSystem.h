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
#include "PlatformSystem.h"


// =====================================================
// 前方宣言　Windows.hを.cpp内だけでインクルードする
// =====================================================
// アプリケーションハンドルの前方宣言
struct APPLICATIONHANDLE;              // HINSTANCEのラップ構造体
// ウィンドウハンドルの前方宣言
struct HWND__;
using HWND = HWND__*;

// DrawManagerに置き換える　---------------------------------
class ShaderManager;          // シェーダーマネージャー
class ConstantBufferManager;  // 定数バッファマネージャー
// ----------------------------------------------------------


// =====================================================
// ウィンドウプラットフォームの初期化・後処理クラス
// =====================================================
class PlatformWindowsSystem : public PlatformSystem
{
private:
	static APPLICATIONHANDLE m_AppInstance;      // アプリケーションのハンドル
	static HWND              m_WinInstance;      // ウィンドウハンドル
	const unsigned int      m_Width;	         // ウィンドウの画面横幅 
	const unsigned int      m_Height;	         // ウィンドウの画面縦幅
	const wchar_t*          m_WindowName;        // ウィンドウの名前
	const wchar_t*          m_WindowClassName;   // ウィンドウのクラス名

	// DrawManagerに置き換える
	static ShaderManager m_ShaderManager;                 // シェーダーを管理する
	static ConstantBufferManager m_ConstantBufferManager; // 定数バッファマネージャー


	// オーバーライド関数
	bool Init()       override; // 初期化処理
	void GameLoop()   override; // ゲームループ
	void Uninit()     override; // 後処理
	bool GameInit()   override; // ゲームの開始処理
	void GameMain()   override; // ゲームのメイン処理
	void GameUninit() override; // ゲームの後処理

public:
	// コンストラクタ
	PlatformWindowsSystem(unsigned int Width, unsigned int Height, const wchar_t* WindowClassName, const wchar_t* WindowName)
		:m_Width(Width), m_Height(Height), m_WindowClassName(WindowClassName), m_WindowName(WindowName) {}

	// デストラクタ
	~PlatformWindowsSystem();

	// ゲッター
	unsigned int GetWidthSize() const  { return m_Width; }   // ウィンドウの横幅を戻り値で返す
	unsigned int GetHeightSize() const { return m_Height; }	 // ウィンドウの縦幅を戻り値で返す
};