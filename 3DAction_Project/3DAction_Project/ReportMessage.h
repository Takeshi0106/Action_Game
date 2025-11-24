#pragma once


// ==========================================================================
// 【関数概要】
// エラー時やデバッグ時にログを出力する補助関数群
// 
// 【設計意図】
// ・プラットフォームごとに.cppを切り替えることで、GameMainなどでも共通的に使用可能
// ・ErrorLogは常時出力、DebugLogはリリースビルド時に無効化される設計
// ・DebugLogの関数はリリース時にインライン化されず空関数となることで、負荷ゼロを実現
// ==========================================================================


// ======================================
// エラーログ（致命的な警告用）
// ======================================
namespace ErrorLog 
{
    // コンソールにメッセージを出力
    void OutputToConsole(const char* message);
    // メッセージボックスでメッセージを表示
    void OutputToMessageBox(const char* message);
}


// =======================================
// 警告ログ (致命的ではない警告用)
// =======================================
namespace WarningLog
{
    // コンソールにメッセージを出力
    void OutputToConsole(const char* message);
    // メッセージボックスで出力
    void OutputToMessageBox(const char* message);
}


// ==============================================================
// デバッグログ
// ここにある関数はリリース時はインラインでビルドを行わないように
// ==============================================================
namespace DebugLog
{
#if defined(DEBUG) || defined(_DEBUG)
    // コンソールにメッセージ出力
    void OutputToConsole(const char* message);

#else
    // リリース時はインラインでビルドしないように
    inline void OutputToConsole(const char* message) {
    }

#endif
}
