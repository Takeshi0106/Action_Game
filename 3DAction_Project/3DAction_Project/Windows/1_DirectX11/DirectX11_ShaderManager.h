#pragma once

// ==============================================
// 【クラス概要】
// DirectX11用シェーダーマネージャー
// すでにコンパイルしているものを探して管理する
// APIObject を管理するマネージャー
// ==============================================


// ==============================================
// ヘッダー
// ==============================================
// シェーダーデータ
#include "DirectX11_ShaderData.h"
// ハンドルヘッダー
#include "../../Handle.h"
// データ管理テンプレートヘッダー
#include "../../TemplateManager.h"
// 文字列ヘッダー
#include "../../UTF8_String.h"


// ==============================================
// クラス
// ==============================================
class DirectX11_ShaderManager
{
private:
	// ------------------------------------------
	// メンバー変数
	// ------------------------------------------
	// シェーダー管理マネージャー
	TemplateManager<DirectX11_VertexShaderData> m_Vertexs;
	TemplateManager<DirectX11_PixelShaderData> m_Pixels;
	TemplateManager<DirectX11_ComputeShaderData> m_Computes;

	// コンパイルファイルパス (コンパイル情報が入っているパス)
	const String& kCompileFilePath;
	// コンパイルファイル拡張子
	const String kCompileFileExt = u8".cso";
	// リファレンス情報を入れる拡張子
	const String kCompileRefExt = u8".ref";


public:
	// ------------------------------------------
	// コンストラクタ・デストラクタ
	// ------------------------------------------
	DirectX11_ShaderManager(const String& _path) :
		kCompileFilePath(_path) {}
	~DirectX11_ShaderManager() = default;


	// ------------------------------------------
	// シェーダー作成関数
	// ------------------------------------------
	Handle VertexShaderCreateOnGet (ID3D11Device* _device, const Hashed_String& _name);
	Handle PixelShaderCreateOnGet  (ID3D11Device* _device, const Hashed_String& _name);
	Handle ComputeShaderCreateOnGet(ID3D11Device* _device, const Hashed_String& _name);


	// ------------------------------------------
	// シェーダー取得関数
	// ------------------------------------------
	// 頂点シェーダー取得
	const DirectX11_VertexShaderData* GetVertexShader(const Handle& _handle) {
		return m_Vertexs.GetData(_handle);
	}
	// ピクセルシェーダー取得
	const DirectX11_PixelShaderData* GetPixelShader(const Handle& _handle) {
		return m_Pixels.GetData(_handle);
	}
	// コンピュートシェーダー取得
	const DirectX11_ComputeShaderData* GetComputeShader(const Handle& _handle) {
		return m_Computes.GetData(_handle);
	}


	// ------------------------------------------
	// シェーダーチェック関数
	// ------------------------------------------
	// 頂点シェーダーチェック
	const bool ExistsVertexShader(const Hashed_String& _name) const {
		return m_Vertexs.Exists(_name);
	}
	// ピクセルシェーダーチェック
	const bool ExistsPixelShader(const Hashed_String& _name) const {
		return m_Pixels.Exists(_name);
	}
	// コンピュートシェーダーチェック
	const bool ExistsComputeShader(const Hashed_String& _name) const {
		return m_Computes.Exists(_name);
	}


	// ------------------------------------------
	// シェーダー削除
	// ------------------------------------------
	// 頂点シェーダー削除
	void ReleaseVertexShader(const Handle& _name) {
		m_Vertexs.Remove(_name);
	}
	// ピクセルシェーダー削除
	void ReleasePixelShader(const Handle& _name) {
		m_Pixels.Remove(_name);
	}
	// コンピュートシェーダー削除
	void ReleaseComputeShader(const Handle& _name) {
		m_Computes.Remove(_name);
	}


	// ------------------------------------------
	// シェーダー全て削除
	// ------------------------------------------
	void ReleaseAllShader() 
	{
		// 全てクリア
		m_Vertexs.ALLClear();
		m_Pixels.ALLClear();
		m_Computes.ALLClear();
	}
};

