#pragma once

// =================================
// 【関数概要】
// バッファで使用する便利関数
// 
// *注意
// 重たいヘッダーをインクルードしています
// Utilsなのでヘッダーにインクルード非推奨
// =================================


// =================================
// ヘッダー
// =================================
#include <d3d11.h>
#include "BufferSetting.h"
#include "ReportMessage.h"

namespace BufferUtils {

    // 使用目的に変換
    inline D3D11_USAGE ToDXUsage(BufferUsage usage)
    {
        switch (usage)
        {
        case BufferUsage::Default: {
            return D3D11_USAGE_DEFAULT;
            break;
        }
        case BufferUsage::Dynamic: {
            return D3D11_USAGE_DYNAMIC;
            break;
        }
        default: {
            ErrorLog::OutputToConsole("BufferUsageに変換できませんでした");
            return D3D11_USAGE_DEFAULT;
        }
        }
    }

    // アクセス制限に変換
    inline UINT ToDXCPUAccess(CPUAccess access)
    {
        switch (access)
        {
        case CPUAccess::None: {
            return static_cast<D3D11_CPU_ACCESS_FLAG>(0);
            break;
        }
        case CPUAccess::Write: {
            return D3D11_CPU_ACCESS_WRITE;
            break;
        }
        default: {
            ErrorLog::OutputToConsole("CPUAccessに変換できませんでした");
            return static_cast<D3D11_CPU_ACCESS_FLAG>(0);
        }
        }
    }
}
