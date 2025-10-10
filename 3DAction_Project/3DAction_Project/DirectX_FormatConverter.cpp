
// =======================================
// ヘッダー
// =======================================
// 標準ヘッダー
#include "DirectX_FormatConverter.h"
// 出力用ヘッダー
#include "ReportMessage.h"


// ==========================
// 使用用途変換
// ==========================
D3D11_USAGE DirectX_FormatConverter::ToDXUsage(BufferUsage usage)
{
    switch (usage)
    {
    case BufferUsage::Default:
        return D3D11_USAGE_DEFAULT;
        break;
    case BufferUsage::Dynamic:
        return D3D11_USAGE_DYNAMIC;
        break;
    default:
        ErrorLog::OutputToConsole("BufferUsageに変換できませんでした");
        return D3D11_USAGE_DEFAULT;
        break;
    }
}


// ============================
// アクセス制限に変換
// ============================
UINT DirectX_FormatConverter::ToDXCPUAccess(CPUAccess access)
{
    switch (access)
    {
    case CPUAccess::None:
        return static_cast<D3D11_CPU_ACCESS_FLAG>(0);
        break;
    case CPUAccess::Write:
        return D3D11_CPU_ACCESS_WRITE;
        break;
    default:
        ErrorLog::OutputToConsole("CPUAccessに変換できませんでした");
        return static_cast<D3D11_CPU_ACCESS_FLAG>(0);
        break;
    }
}


// ===========================
// 自作Format から DXGI_FORMAT に変換
// ===========================
DXGI_FORMAT DirectX_FormatConverter::ToDXFormat(Format format)
{
    switch (format)
    {
    case Format::Unknown:
        return DXGI_FORMAT_UNKNOWN;
        break;
    case Format::R8G8B8A8_UNorm:
        return DXGI_FORMAT_R8G8B8A8_UNORM;
        break;
    case Format::B8G8R8A8_UNorm_SRGB:
        return DXGI_FORMAT_B8G8R8A8_UNORM_SRGB;
        break;
    case Format::R32_Float:
        return DXGI_FORMAT_R32_FLOAT;
        break;
    case Format::R32G32B32_Float:
        return DXGI_FORMAT_R32G32B32_FLOAT;
        break;
    case Format::R32G32B32A32_Float:
        return DXGI_FORMAT_R32G32B32A32_FLOAT;
        break;
    case Format::D24_UNorm_S8_UInt:
        return DXGI_FORMAT_D24_UNORM_S8_UINT;
        break;
    default:
        ErrorLog::OutputToConsole("Formatに変換できませんでした");
        return DXGI_FORMAT_UNKNOWN;
        break;
    }
}


// ===========================
// DXGI_FORMAT から 自作Format に変換
// ===========================
Format DirectX_FormatConverter::ToSelfFormat(DXGI_FORMAT format)
{
    switch (format)
    {
    case DXGI_FORMAT_UNKNOWN:
        return Format::Unknown;
        break;
    case DXGI_FORMAT_R8G8B8A8_UNORM:
        return Format::R8G8B8A8_UNorm;
        break;
    case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB:
        return Format::B8G8R8A8_UNorm_SRGB;
        break;
    case DXGI_FORMAT_R32_FLOAT:
        return Format::R32_Float;
        break;
    case DXGI_FORMAT_R32G32B32_FLOAT:
        return Format::R32G32B32_Float;
        break;
    case DXGI_FORMAT_R32G32B32A32_FLOAT:
        return Format::R32G32B32A32_Float;
        break;
    case DXGI_FORMAT_D24_UNORM_S8_UINT:
        return Format::D24_UNorm_S8_UInt;
        break;
    default:
        ErrorLog::OutputToConsole("Formatに変換できませんでした");
        return Format::Unknown;
        break;
    }
}


D3D11_FILTER DirectX_FormatConverter::ConvertFilter(SamplerFilter filter)
{
    switch (filter)
    {
    case SamplerFilter::Point: 
        return D3D11_FILTER_MIN_MAG_MIP_POINT;
        break;
    case SamplerFilter::Linear: 
        return D3D11_FILTER_MIN_MAG_MIP_LINEAR;
        break;
    case SamplerFilter::Anisotropic: 
        return D3D11_FILTER_ANISOTROPIC;
        break;
    default: 
        ErrorLog::OutputToConsole("D3D11_FILTER に変換できませんでした");
        return D3D11_FILTER_MIN_MAG_MIP_LINEAR;
        break;
    }
}

D3D11_TEXTURE_ADDRESS_MODE DirectX_FormatConverter::ConvertAddressMode(SamplerAddressMode mode)
{
    switch (mode)
    {
    case SamplerAddressMode::Wrap: 
        return D3D11_TEXTURE_ADDRESS_WRAP;
        break;
    case SamplerAddressMode::Mirror: 
        return D3D11_TEXTURE_ADDRESS_MIRROR;
        break;
    case SamplerAddressMode::Clamp: 
        return D3D11_TEXTURE_ADDRESS_CLAMP;
        break;
    case SamplerAddressMode::Border: 
        return D3D11_TEXTURE_ADDRESS_BORDER;
        break;
    default: 
        ErrorLog::OutputToConsole("D3D11_TEXTURE_ADDRESS_MODE に変換できませんでした");
        return D3D11_TEXTURE_ADDRESS_WRAP;
        break;
    }
}

D3D11_COMPARISON_FUNC DirectX_FormatConverter::ConvertComparisonFunc(SamplerComparisonFunc func)
{
    switch (func)
    {
    case SamplerComparisonFunc::Never: 
        return D3D11_COMPARISON_NEVER;
        break;
    case SamplerComparisonFunc::Less: 
        return D3D11_COMPARISON_LESS;
        break;
    case SamplerComparisonFunc::Equal: 
        return D3D11_COMPARISON_EQUAL;
        break;
    case SamplerComparisonFunc::LessEqual: 
        return D3D11_COMPARISON_LESS_EQUAL;
        break;
    case SamplerComparisonFunc::Greater: 
        return D3D11_COMPARISON_GREATER;
        break;
    case SamplerComparisonFunc::NotEqual: 
        return D3D11_COMPARISON_NOT_EQUAL;
        break;
    case SamplerComparisonFunc::GreaterEqual:
        return D3D11_COMPARISON_GREATER_EQUAL;
        break;
    case SamplerComparisonFunc::Always:
        return D3D11_COMPARISON_ALWAYS;
        break;
    default: 
        ErrorLog::OutputToConsole("D3D11_COMPARISON_FUNC に変換できませんでした");
        return D3D11_COMPARISON_ALWAYS;
        break;
    }
}