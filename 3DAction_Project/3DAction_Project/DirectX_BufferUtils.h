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

    // フォーマットを変換 
    inline  DXGI_FORMAT toDXFormat(Format format)
    {
        switch (format)
        {
        case Format::Unknown: {
            return DXGI_FORMAT_UNKNOWN;
            break;
        }
        case Format::R8G8B8A8_UNorm: {
            return DXGI_FORMAT_R8G8B8A8_UNORM;
            break;
        }
        case Format::R32_Float: {
            return DXGI_FORMAT_R32_FLOAT;
            break;
        }
        case Format::R32G32B32_Float: {
            return DXGI_FORMAT_R32G32B32_FLOAT;
            break;
        }
        case Format::R32G32B32A32_Float: {
            return DXGI_FORMAT_R32G32B32A32_FLOAT;
            break;
        }
        case Format::D24_UNorm_S8_UInt: {
            return DXGI_FORMAT_D24_UNORM_S8_UINT;
            break;
        }

        default:
            ErrorLog::OutputToConsole("Formatに変換できませんでした");
            return DXGI_FORMAT_UNKNOWN;
        }
    }


}
