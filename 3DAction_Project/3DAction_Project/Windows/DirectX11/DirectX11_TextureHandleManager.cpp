
// ======================================
// ヘッダー
// ======================================
// 必須ヘッダー
#include "DirectX11_TextureHandleManager.h"
// DirectX11ヘッダー
#include <d3d11.h>
// ログ出力用ヘッダー
#include "../../ReportMessage.h"


// ===============================
// テクスチャ作成
// ===============================
Handle DirectX11_TextureHandleManager::CreateTextures(
	ID3D11Device* _device,
	const Hashed_String& _name,
	const D3D11_TEXTURE2D_DESC& _desc,
	const D3D11_SAMPLER_DESC& _dxDesc,
	const SamplerDesc& _myDesc,
	const D3D11_SHADER_RESOURCE_VIEW_DESC* _srvDesc,
	const D3D11_RENDER_TARGET_VIEW_DESC* _rtvDesc,
	const D3D11_DEPTH_STENCIL_VIEW_DESC* _dsvDesc)
{
	if (m_TextureHandles.Exists(_name))
	{
		// ハンドル取得
		Handle handle = m_TextureHandles.GetHandle(_name);

#if defined(DEBUG) || defined(_DEBUG)
		// 同一確認
		const TextureHandle* existingTextureHandle = m_TextureHandles.GetData(handle);

		// nullチェック
		if (existingTextureHandle == nullptr)
		{
			// ログ出力
			WarningLog::OutputToConsole(
				u8"同じ名前のテクスチャが既に作成されていましたが、データが見つかりませんでした: " +
				_name.GetString());

			return Handle();
		}

		// 各種ビューの同一確認
		// SRV
		if (_srvDesc != nullptr)
		{
			if (!existingTextureHandle->srvHandle.IsValid())
			{
				WarningLog::OutputToConsole(
					u8"同じ名前のテクスチャが異なるSRV設定で作成されようとしました: " +
					_name.GetString());
				return Handle();
			}
		}
		// RTV
		if (_rtvDesc != nullptr)
		{
			if (!existingTextureHandle->rtvHandle.IsValid())
			{
				WarningLog::OutputToConsole(
					u8"同じ名前のテクスチャが異なるRTV設定で作成されようとしました: " +
					_name.GetString());
				return Handle();
			}
		}
		// DSV
		if (_dsvDesc != nullptr)
		{
			if (!existingTextureHandle->dsvHandle.IsValid())
			{
				WarningLog::OutputToConsole(
					u8"同じ名前のテクスチャが異なるDSV設定で作成されようとしました: " +
					_name.GetString());
				return Handle();
			}
		}
#endif

		// ログ出力
		WarningLog::OutputToConsole(
			u8"同じ名前のテクスチャが既に作成されていました: " +
			_name.GetString());

		// ハンドル返す
		return handle;
	}

	// 無効チェック
	if (!_device) {
		ErrorLog::OutputToConsole(u8"無効なデバイスが渡されました。");
		return Handle();
	}

	// テクスチャ作成
	TextureHandle textureHandle;

	// テクスチャバッファ作成
	textureHandle.textureHandle = m_Texture2DBufferManager.Texture2DBufferCreateOnGet(
		_device,
		_desc,
		_name);

	// サンプラーステート作成済みかチェック
	if (m_SamplerManager.Exists(_myDesc))
	{
		WarningLog::OutputToConsole(
			u8"同じ設定のサンプラーが作成されようとしました");

		// 取得
		textureHandle.samplerHandle = m_SamplerManager.GetHandle(_myDesc);
	}
	else
	{
		// サンプラーステート作成
		textureHandle.samplerHandle = m_SamplerManager.SamplerStateCreateOnGet(
			_device,
			_dxDesc,
			_myDesc);
	}

	// シェーダーリソースビュー作成
	if (_srvDesc)
	{
		textureHandle.srvHandle = m_ViewManager.ShaderResourceViewCreateOnGet(
			_device,
			*m_Texture2DBufferManager.GetTexture2DBuffer(textureHandle.textureHandle)->GetTexture(),
			*_srvDesc,
			_name);
	}
	// レンダーターゲットビュー作成
	if (_rtvDesc)
	{
		textureHandle.rtvHandle = m_ViewManager.RenderTargetViewCreateOnGet(
			_device,
			*m_Texture2DBufferManager.GetTexture2DBuffer(textureHandle.textureHandle)->GetTexture(),
			*_rtvDesc,
			_name);
	}
	// 深度ステンシルビュー作成
	if (_dsvDesc)
	{
		textureHandle.dsvHandle = m_ViewManager.DepthStencilViewCreateOnGet(
			_device,
			*m_Texture2DBufferManager.GetTexture2DBuffer(textureHandle.textureHandle)->GetTexture(),
			*_dsvDesc,
			_name);
	}

	// 管理配列に追加してハンドルを返す
	return m_TextureHandles.AddData(_name, textureHandle);
}


// ===============================
// テクスチャロード (ファイルから)
// ===============================
Handle DirectX11_TextureHandleManager::LoadFaileTexture_TextureFolder(
	ID3D11Device* _device,
	const Hashed_String& _name,
	const String& _filePath)
{
	// 既に存在する場合はハンドルを返す
	if (m_TextureHandles.Exists(_name))
	{
		// ログ出力
		WarningLog::OutputToConsole(
			u8"同じ名前のテクスチャが既に作成されていました: " +
			_name.GetString());

		// ハンドル返す
		return m_TextureHandles.GetHandle(_name);
	}

	// 無効チェック
	if (!_device) {
		ErrorLog::OutputToConsole(u8"無効なデバイスが渡されました。");
		return Handle();
	}

	// テクスチャロード
	TextureHandle textureHandle;

	// テクスチャロードモジュールに処理を任せる
	textureHandle = m_TextureLoadModule.LoadFaileTexture_TextureFolder(
		_device,
		_name,
		_filePath,
		m_Texture2DBufferManager,
		m_ViewManager);

	// 管理配列に追加してハンドルを返す
	return m_TextureHandles.AddData(_name, textureHandle);
}


// ===============================
// テクスチャロード (プロジェクトの相対パスから)
// ===============================
Handle DirectX11_TextureHandleManager::LoadFaileTexture(
	ID3D11Device* _device,
	const Hashed_String& _name,
	const String& _filePath)
{
	// 既に存在する場合はハンドルを返す
	if (m_TextureHandles.Exists(_name))
	{
		// ログ出力
		WarningLog::OutputToConsole(
			u8"同じ名前のテクスチャが既に作成されていました: " +
			_name.GetString());

		// ハンドル返す
		return m_TextureHandles.GetHandle(_name);
	}

	// 無効チェック
	if (!_device) {
		ErrorLog::OutputToConsole(u8"無効なデバイスが渡されました。");
		return Handle();
	}

	// テクスチャロード
	TextureHandle textureHandle;

	// テクスチャロードモジュールに処理を任せる
	textureHandle = m_TextureLoadModule.LoadFaileTexture(
		_device,
		_name,
		_filePath,
		m_Texture2DBufferManager,
		m_ViewManager);

	// 管理配列に追加してハンドルを返す
	return m_TextureHandles.AddData(_name, textureHandle);
}
