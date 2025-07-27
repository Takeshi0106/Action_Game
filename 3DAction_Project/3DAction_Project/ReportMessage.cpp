#include "ReportMessage.h"
#include <Windows.h> // メッセージボックス出力、HRESULT出力

#if defined(DEBUG) || defined(_DEBUG)
#include <iostream> // デバッグ文字を出力ウィンドウに書き出す
#endif


namespace ErrorLog {

    // HRESULTの成否とメッセージボックスとデバッグ時のみログを出力する
    bool IsHRESULTFailedWithLog(long longHr, const char* message)
    {
        HRESULT hr = static_cast<HRESULT>(longHr); // HRESULTにキャストする コンパイル時に変換が決まる

        if (FAILED(hr))
        {
#if defined(DEBUG) || defined(_DEBUG)

            std::cerr << "Error HRESULT: " << hr << std::endl; // HRESULTエラーを出力

#endif
            MessageBoxA(nullptr, message, "エラー", MB_OK | MB_ICONERROR); // メッセージボックスで出力

            return true;
        }

        return false;
    }


    // メッセージボックスを出力する関数
    void MessageBoxOutput(const char* message)
    {
        MessageBoxA(nullptr, message, "エラー", MB_OK | MB_ICONERROR); // メッセージボックスを出力
    }


#if defined(DEBUG) || defined(_DEBUG)
    // インラインできるようにする
    void Log(const char* message) 
    {
        std::cerr << message << std::endl;
    }
#endif


}