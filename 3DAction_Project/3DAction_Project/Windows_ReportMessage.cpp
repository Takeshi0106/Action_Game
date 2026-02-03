
// ======================================
// 【関数概要】
// Windows用_エラーログ出力関数群
// ======================================


// ======================================
// ヘッダー
// ======================================
// 必須ヘッダー
#include "ReportMessage.h"
// メッセージボックス用ヘッダー
#include <Windows.h> // メッセージボックス出力
// コンソール出力用ヘッダー
#include <iostream>  // コンソールに書き出す
// 停止ヘッダー
#include <cstdlib>


// ======================================
// 関数
// ======================================
// String を char* に変換する関数
inline const char* StringToChar(const String& str)
{
    // 内部の u8string を char* にキャスト
    return reinterpret_cast<const char*>(str.GetU8Char());
}

// String を std::wstring に変換する関数
inline std::wstring StringToWString(const String& str)
{
	// 文字数を取得
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, StringToChar(str), -1, nullptr, 0);
	// std::wstring に変換バッファを確保
    std::wstring wstrTo(size_needed, 0);
	// 変換して代入
    MultiByteToWideChar(CP_UTF8, 0, StringToChar(str), -1, &wstrTo[0], size_needed);
    return wstrTo;
}


// ======================================
// グローバル変数
// ======================================
namespace {
    // カラーコードリセット用
    constexpr const char8_t* RESET = u8"\x1b[0m";
}


// ======================================
// エラーログ（致命的な警告）
// ======================================
namespace ErrorLog
{
    // カラーコード
    constexpr const char8_t* RED = u8"\x1b[31m";


    // ====================================================
    // コンソールにメッセージを出力
    // ====================================================
    void OutputToConsole(const String& message)
    {
		// 赤色でエラーメッセージを表示
		String output = String(RED) + u8"ERROR :" + message + RESET;
        std::cerr << StringToChar(output) << std::endl;

#if defined(DEBUG) || defined(_DEBUG)
        // プロジェクト停止
        std::abort();
#endif
    }


    // =====================================================
    // メッセージボックスでメッセージを表示
    // =====================================================
    void OutputToMessageBox(const String& message)
    {
        MessageBoxW(nullptr, StringToWString(message).c_str(), L"Error", MB_ICONERROR | MB_OK);

#if defined(DEBUG) || defined(_DEBUG)
        // プロジェクト停止
        std::abort();
#endif
    }


}


// ===========================================
// 警告ログ (致命的ではないエラーログ)
// ===========================================
namespace WarningLog
{
    // ANSIカラーコード
    constexpr const char8_t* YELLOW = u8"\x1b[33m";


    // ====================================================
    // コンソールにメッセージを出力
    // ====================================================
    void OutputToConsole(const String& message)
    {
		// 警告メッセージ作成
		String output = String(YELLOW) + u8"WARNING :" + message + RESET;
		// 黄色で警告メッセージを表示
        std::cerr << StringToChar(output) << std::endl;
    }


    // =====================================================
    // メッセージボックスでメッセージを表示
    // =====================================================
    void OutputToMessageBox(const String& message)
    {
        MessageBoxW(nullptr, StringToWString(message).c_str(), L"Warning", MB_ICONERROR | MB_OK);
    }


}


// ==============================================================
// デバッグログ
// ここにある関数はリリース時はインラインでビルドを行わないように
// ==============================================================
namespace DebugLog
{
    // ===============================================
    // コンソールにメッセージ出力
    // ===============================================
    void OutputToConsole(const String& message)
    {
        std::cout << StringToChar(message) << std::endl;
    }


}


