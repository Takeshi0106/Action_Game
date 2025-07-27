#pragma once

namespace ErrorLog {
bool IsHRESULTFailedWithLog(long hr, const char* message); // HRESULTとメッセージボックスで出力するメッセージを引き数で渡す　失敗時に戻り値はtrueを返す
void MessageBoxOutput(const char* message);                // メッセージボックスを出力させる

#if defined(DEBUG) || defined(_DEBUG)
void Log(const char* message); // 実態を.cpp に記載する
#else
inline void Log(const char* message){} // インラインにしてなにも処理されないようにする
#endif


}