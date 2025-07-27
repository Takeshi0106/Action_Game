#pragma once


#if defined(DEBUG) || defined(_DEBUG)
#include <iostream> // デバッグ文字を出力ウィンドウに書き出す
#endif

namespace ErrorLog {
bool IsHRESULTFailedWithLog(long hr, const char* message); // HRESULTとメッセージボックスで出力するメッセージを引き数で渡す　失敗時に戻り値はtrueを返す


#if defined(DEBUG) || defined(_DEBUG)
inline void Log(const char* message)
{
    std::cerr << message << std::endl;
}
#else
inline void Log(const char* message){}
#endif


}