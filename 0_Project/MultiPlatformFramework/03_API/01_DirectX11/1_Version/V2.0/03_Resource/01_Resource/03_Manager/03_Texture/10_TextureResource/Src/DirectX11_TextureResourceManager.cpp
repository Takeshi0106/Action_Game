#include "DirectX11_TextureResourceManager.h"
#include <d3d11.h>
#include "ReportMessage.h"

// テクスチャ作成
Handle DirectX11_TextureResourceManager::CreateTextures(
	ID3D11Device* _device,
	const Hashed_String& _name,
	const D3D11_TEXTURE2D_DESC& _desc,
	const D3D11_SHADER_RESOURCE_VIEW_DESC* _srvDesc,
	const D3D11_RENDER_TARGET_VIEW_DESC* _rtvDesc,
	const D3D11_DEPTH_STENCIL_VIEW_DESC* _dsvDesc)
{
	// すでにあるか確認
	if (m_TextureHandles.Exists(_name))
	{
		Handle handle = m_TextureHandles.GetHandle(_name);
#if defined(DEBUG) || defined(_DEBUG)
		// 同一確認
		const TextureHandle* existingTextureHandle = m_TextureHandles.GetData(handle);
		if (existingTextureHandle == nullptr)
		{
			// ログ出力
			ErrorLog::OutputToConsole(u8"同じ名前のテクスチャが既に作成されていましたが、データが見つかりませんでした: " + _name.GetString());
			return Handle();
		}
		if (_srvDesc != nullptr)
		{
			if (!existingTextureHandle->srvHandle.IsValid())
			{
				ErrorLog::OutputToConsole(u8"同じ名前のテクスチャでSRVが作成されようとしました: " + _name.GetString());
				return Handle();
			}
		}
		if (_rtvDesc != nullptr)
		{
			if (!existingTextureHandle->rtvHandle.IsValid())
			{
				ErrorLog::OutputToConsole(u8"同じ名前のテクスチャでRTVが作成されようとしました: " + _name.GetString());
				return Handle();
			}
		}
		if (_dsvDesc != nullptr)
		{
			if (!existingTextureHandle->dsvHandle.IsValid())
			{
				ErrorLog::OutputToConsole(u8"同じ名前のテクスチャでDSVが作成されようとしました: " + _name.GetString());
				return Handle();
			}
		}
#endif
		DebugLog::OutputToConsole(u8"同じ名前のテクスチャが既に作成されていました: " + _name.GetString());
		return handle;
	}
	
	// テクスチャ作成
	TextureHandle textureHandle;
	textureHandle.textureHandle = m_Texture2DBufferManager.Texture2DBufferCreateOnGet(
		_device,
		_desc,
		_name);

	// ビュー作成	
	const DirectX11_Texture2DBufferData* texture = m_Texture2DBufferManager.GetTexture2DBuffer(textureHandle.textureHandle);
	if (_srvDesc)
	{
		textureHandle.srvHandle = m_ViewManager.ShaderResourceViewCreateOnGet(
			_device,
			*texture->GetTexture(),
			*_srvDesc,
			_name);
	}
	if (_rtvDesc)
	{
		textureHandle.rtvHandle = m_ViewManager.RenderTargetViewCreateOnGet(
			_device,
			*texture->GetTexture(),
			*_rtvDesc,
			_name);
	}
	if (_dsvDesc)
	{
		textureHandle.dsvHandle = m_ViewManager.DepthStencilViewCreateOnGet(
			_device,
			*texture->GetTexture(),
			*_dsvDesc,
			_name);
	}

	// 管理配列に追加してハンドルを返す
	return m_TextureHandles.AddData(_name, textureHandle);
}


// ===============================
// テクスチャロード (ファイルから)
// ===============================
Handle DirectX11_TextureResourceManager::LoadFileTexture_TextureFolder(
	ID3D11Device* _device,
	ID3D11DeviceContext* _deviceContext,
	const Hashed_String& _name,
	const TextureLoadDesc& _desc,
	const String& _filePath)
{
	// 既に存在する場合はハンドルを返す
	if (m_TextureHandles.Exists(_name))
	{
		DebugLog::OutputToConsole(u8"同じ名前のテクスチャが既に作成されていました: " +_name.GetString());
		return m_TextureHandles.GetHandle(_name);
	}

	// テクスチャロード
	TextureHandle textureHandle;
	textureHandle = m_TextureLoadModule.LoadFileTexture_TextureFolder(
		_device,
		_deviceContext,
		_name,
		_filePath,
		_desc,
		m_Texture2DBufferManager,
		m_ViewManager);

	// 管理配列に追加してハンドルを返す
	return m_TextureHandles.AddData(_name, textureHandle);
}


// ===============================
// テクスチャロード (プロジェクトの相対パスから)
// ===============================
Handle DirectX11_TextureResourceManager::LoadFileTexture(
	ID3D11Device* _device,
	ID3D11DeviceContext* _deviceContext,
	const Hashed_String& _name,
	const String& _filePath)
{
	// 既に存在する場合はハンドルを返す
	if (m_TextureHandles.Exists(_name))
	{
		DebugLog::OutputToConsole(u8"同じ名前のテクスチャが既に作成されていました: " + _name.GetString());
		return m_TextureHandles.GetHandle(_name);
	}

	// テクスチャロード
	TextureHandle textureHandle;

	// テクスチャロードモジュールに処理を任せる
	textureHandle = m_TextureLoadModule.LoadFileTexture(
		_device,
		_deviceContext,
		_name,
		_filePath,
		m_Texture2DBufferManager,
		m_ViewManager);

	// 管理配列に追加してハンドルを返す
	return m_TextureHandles.AddData(_name, textureHandle);
}
