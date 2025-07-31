#include "ReportMessage.h"
#include <Windows.h> // メッセージボックス出力、HRESULT出力
#include <iostream> // デバッグ文字を出力ウィンドウに書き出す


namespace ErrorLog {

    // HRESULTの成否とメッセージボックスとデバッグ時のみログを出力する
    bool IsSuccessHRESULTWithMessageBox(long longHr, const char* message)
    {
        HRESULT hr = static_cast<HRESULT>(longHr); // HRESULTにキャストする コンパイル時に変換が決まる

        if (FAILED(hr))
        {
            MessageBoxA(nullptr, message, "エラー", MB_OK | MB_ICONERROR); // メッセージボックスで出力
#if defined(DEBUG) || defined(_DEBUG)

            std::cerr << message << std::endl;
            std::cerr << "Error HRESULT: " << hr << std::endl; // HRESULTエラーを出力
            std::cerr << std::endl;
#endif

            return false;
        }

        return true;
    }


    // HRの判定と、文字出力
    bool IsSuccessHRESULTWithOutputToConsole(long longhr, const char* message)
    {
        HRESULT hr = static_cast<HRESULT>(longhr); // HRESULTにキャストする コンパイル時に変換が決まる

        if (FAILED(hr))
        {
#if defined(DEBUG) || defined(_DEBUG)
            std::cerr << message << std::endl;
            std::cerr << "Error HRESULT: " << hr << std::endl; // HRESULTエラーを出力
#endif

            return false;
        }

        return true;
    }


    // メッセージボックスを出力する関数
    void MessageBoxOutput(const char* message)
    {
        MessageBoxA(nullptr, message, "エラー", MB_OK | MB_ICONERROR); // メッセージボックスを出力
    }

    // インラインできるようにする
    void Log(const char* message)
    {
        std::cerr << message << std::endl;
    }
}

namespace Debug {

#if defined(DEBUG) || defined(_DEBUG)
    void NormalLog(const char* message)
    {
        std::cout << message << std::endl;
    }
#endif
}