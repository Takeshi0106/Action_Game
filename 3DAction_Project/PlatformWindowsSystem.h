#pragma once
#include "PlatformSystem.h"
#include <string>


// =====================================================
// 前方宣言
// =====================================================
// アプリケーションハンドルの前方宣言
struct APPLICATIONHANDLE;              // HINSTANCEのラップ構造体
// ウィンドウハンドルの前方宣言
struct WINDOWHANDLE;                   // HWNDのラップ構造体

// =====================================================
// ウィンドウプラットフォームの初期化・後処理クラス
// =====================================================
class PlatformWindowsSystem : public PlatformSystem
{
private:
	static APPLICATIONHANDLE m_AppInstance; // アプリケーションのハンドル
	static WINDOWHANDLE m_WinInstance;      // ウィンドウハンドル
	static uint16_t  m_Width;	            // ウィンドウの画面横幅 
	static uint16_t  m_Height;	            // ウィンドウの画面縦幅
	static std::wstring m_WindowName;       // ウィンドウの名前
	static std::wstring m_WindowClassName;  // ウィンドウのクラス名

	bool Init() override;     // 初期化処理
	void GameLoop() override; // ゲームループ
	void Uninit() override;   // 後処理

public:
	// コンストラクタ・デストラクタ
	PlatformWindowsSystem(uint16_t Width, uint16_t Height, std::wstring WindowClassName, std::wstring WindowName);
	~PlatformWindowsSystem();

	// ゲッター
	static uint16_t GetWidthSize() { return m_Width; }       // ウィンドウの横幅を戻り値で返す
	static uint16_t GetHeightSize() { return m_Height; }	 // ウィンドウの縦幅を戻り値で返す
};