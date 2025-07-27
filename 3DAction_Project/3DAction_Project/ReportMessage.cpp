#include "ReportMessage.h"
#include <Windows.h> // メッセージボックス出力、HRESULT出力

namespace ErrorLog {

    bool IsHRESULTFailedWithLog(long longHr, const char* message) // HRESULTログを出力する
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


}