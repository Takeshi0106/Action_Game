#pragma once

namespace ErrorLog {
bool IsSuccessHRESULTWithMessageBox(long hr, const char* message); // HRESULTとメッセージボックスで出力するメッセージを引き数で渡す　失敗時に戻り値はtrueを返す
bool IsSuccessHRESULTWitchOutputToConsole(long hr, const char* message); // hrの判定と、失敗時にデバッグ時のみコンソールに出力
void MessageBoxOutput(const char* message);                // メッセージボックスを出力させる

#if defined(DEBUG) || defined(_DEBUG)
void Log(const char* message); // 実態を.cpp に記載する
#else
inline void Log(const char* message){} // インラインにしてなにも処理されないようにする
#endif


}