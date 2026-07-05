
// =================================
// ヘッダー
// =================================
// 必須ヘッダー
#include "ResourceViewData.h"
// レポートヘッダー
#include "ReportMessage.h"
// 文字列ヘッダー
#include <string>

// ===============================
// SRV
// ===============================
bool SRVData::CreateSRV(ID3D11Device* device,
    ID3D11Texture2D* resource,
    DXGI_FORMAT format,
    UINT mostDetailedMip,
    UINT mipLevels)
{
    // 設定
    D3D11_SHADER_RESOURCE_VIEW_DESC desc{};
    desc.Format = format;
    desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    desc.Texture2D.MostDetailedMip = mostDetailedMip;
    desc.Texture2D.MipLevels = mipLevels;

    // 作成
    HRESULT hr = device->CreateShaderResourceView(resource, &desc, m_SRV.GetAddressOf());
    if (FAILED(hr)) {
        ErrorLog::OutputToConsole(u8"SRVが作成できませんでした " + String::to_u8string((uint64_t)hr));
        return false;
    }

    return true;
}


// ================================
// UAV
// ================================
bool UAVData::CreateUAV(ID3D11Device* device, 
    ID3D11Texture2D* resource,
    DXGI_FORMAT format,
    UINT mipSlice)
{
    // 設定
    D3D11_UNORDERED_ACCESS_VIEW_DESC desc{};
    desc.Format = format;
    desc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
    desc.Texture2D.MipSlice = mipSlice;

    // 作成
    HRESULT hr = device->CreateUnorderedAccessView(resource, &desc, m_UAV.GetAddressOf());
    if (FAILED(hr)) {
        ErrorLog::OutputToConsole(u8"UAVが作成できませんでした " + String::to_u8string((uint64_t)hr));
        return false;
    }

    return true;
}


// ==================================
// RTV
// ==================================
bool RTVData::CreateRTV(ID3D11Device* device,
    ID3D11Texture2D* resource,
    UINT mipSlice)
{
	// テクスチャ情報取得
    D3D11_TEXTURE2D_DESC textureDesc;
    resource->GetDesc(&textureDesc);
    m_Width = textureDesc.Width;
    m_Height = textureDesc.Height;

    // 設定
    D3D11_RENDER_TARGET_VIEW_DESC desc{};
    desc.Format = textureDesc.Format;
    desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	desc.Texture2D.MipSlice = mipSlice;

    // 作成
    HRESULT hr = device->CreateRenderTargetView(resource, &desc, m_RTV.GetAddressOf());
    if (FAILED(hr)) {
        ErrorLog::OutputToConsole(u8"RTVが作成できませんでした " + String::to_u8string((uint64_t)hr));
        return false;
    }

    return true;
}


// ==================================
// DSV
// ==================================
bool DSVData::CreateDSV(ID3D11Device* device,
    ID3D11Texture2D* resource,
    DXGI_FORMAT format)
{
    // 設定
    D3D11_DEPTH_STENCIL_VIEW_DESC desc{};
    desc.Format = format;
    desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

    // 作成
    HRESULT hr = device->CreateDepthStencilView(resource, &desc, m_DSV.GetAddressOf());
    if (FAILED(hr)) {
        ErrorLog::OutputToConsole(u8"DSVが作成できませんでした " + String::to_u8string((uint64_t)hr));
        return false;
    }

    return true;
}
