#pragma once

// =================================
// 【クラス概要】
// DirectXのバッファ関連の列挙型変換を行うユーティリティクラス
//
// *注意
// 重たいヘッダーをインクルードしています
// ヘッダーにインクルード非推奨
// =================================


// =================================
// ヘッダー
// =================================
#include <d3d11.h>
#include "GraphicsEnums.h"


// =================================
// クラス
// =================================
class DirectX_FormatConverter
{
public:
    // 使用目的に変換
    static D3D11_USAGE ToDXUsage(BufferUsage usage);

    // アクセス制限に変換
    static UINT ToDXCPUAccess(CPUAccess access);

    // 自作Format から DXGI_FORMAT に変換
    static DXGI_FORMAT ToDXFormat(Format format);

    // DXGI_FORMAT から 自作Format に変換
    static Format ToSelfFormat(DXGI_FORMAT format);

    // サンプラー変換
    static D3D11_FILTER ConvertFilter(SamplerFilter filter);
    static D3D11_TEXTURE_ADDRESS_MODE ConvertAddressMode(SamplerAddressMode mode);
    static D3D11_COMPARISON_FUNC ConvertComparisonFunc(SamplerComparisonFunc func);
};

