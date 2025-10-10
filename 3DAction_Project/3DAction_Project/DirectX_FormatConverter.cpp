
// =======================================
// �w�b�_�[
// =======================================
// �W���w�b�_�[
#include "DirectX_FormatConverter.h"
// �o�͗p�w�b�_�[
#include "ReportMessage.h"


// ==========================
// �g�p�p�r�ϊ�
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
        ErrorLog::OutputToConsole("BufferUsage�ɕϊ��ł��܂���ł���");
        return D3D11_USAGE_DEFAULT;
        break;
    }
}


// ============================
// �A�N�Z�X�����ɕϊ�
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
        ErrorLog::OutputToConsole("CPUAccess�ɕϊ��ł��܂���ł���");
        return static_cast<D3D11_CPU_ACCESS_FLAG>(0);
        break;
    }
}


// ===========================
// ����Format ���� DXGI_FORMAT �ɕϊ�
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
        ErrorLog::OutputToConsole("Format�ɕϊ��ł��܂���ł���");
        return DXGI_FORMAT_UNKNOWN;
        break;
    }
}


// ===========================
// DXGI_FORMAT ���� ����Format �ɕϊ�
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
        ErrorLog::OutputToConsole("Format�ɕϊ��ł��܂���ł���");
        return Format::Unknown;
        break;
    }
}
