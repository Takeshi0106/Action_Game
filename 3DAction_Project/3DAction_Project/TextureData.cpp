
// =======================================
// ヘッダー
// =======================================
#include "TextureData.h"
#include "ReportMessage.h"
#include <string>


// =======================================
// スワップチェインからテクスチャ作成
// =======================================
bool SwapchainTextureData::CreateTextureFromSwapChain(IDXGISwapChain* swapChain)
{
    // スワップチェインが生成したバックバッファを取得する
    HRESULT hr = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)m_Texture.GetAddressOf());
    if (FAILED(hr)) {
        ErrorLog::OutputToConsole("スワップチェインからバックバッファを取得することが出来ませんでした。");
        return false;
    }

    return true;
}


// =======================================
// テクスチャ作成関数
// =======================================
bool BindableTextureData::CreateTexture2D(ID3D11Device* device,
	unsigned int width,
	unsigned int height,
	DXGI_FORMAT format,
    D3D11_BIND_FLAG bindFlags,
    D3D11_USAGE usage,
    D3D11_CPU_ACCESS_FLAG flag,
    D3D11_SUBRESOURCE_DATA* initData)
{
    // テクスチャ記述
    D3D11_TEXTURE2D_DESC desc{};
    desc.Width = width;
    desc.Height = height;
    desc.MipLevels = 1;
    desc.ArraySize = 1;
    desc.Format = format;
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;
    desc.Usage = usage;
    desc.BindFlags = bindFlags;
    desc.CPUAccessFlags = flag;
    desc.MiscFlags = 0;

    // テクスチャ作成
    HRESULT hr = device->CreateTexture2D(&desc, initData, m_Texture.GetAddressOf());
    if (FAILED(hr)) {
        ErrorLog::OutputToConsole(("テクスチャの作成に失敗しました " + std::to_string(hr)).c_str());
        return false;
    }

	return true;
}
