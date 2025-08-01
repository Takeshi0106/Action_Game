#pragma once

// ========================================
// この関数群の説明
// ========================================
// エラー時などにログを出力する関数群
// プラットフォーム別で.cppを切り替えて
// GameMainでも使用できるようにする


// ======================================
// エラーログ（致命的または警告）
// ======================================
namespace ErrorLog 
{
    // コンソールにメッセージを出力
    void OutputToConsole(const char* message);

    // メッセージボックスでメッセージを表示
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