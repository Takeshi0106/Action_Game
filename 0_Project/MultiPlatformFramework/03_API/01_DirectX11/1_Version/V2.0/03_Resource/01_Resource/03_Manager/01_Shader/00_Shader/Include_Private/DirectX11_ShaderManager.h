#pragma once

// ==============================================
// 【クラス概要】
// DirectX11用シェーダーマネージャー
// すでにコンパイルしているものを取得して管理する
// APIObject を管理するマネージャー
// ==============================================
#include "DirectX11_ShaderData.h"
#include "TemplateManager.h"
#include "Handle.h"
#include "UTF8_String.h"

// ==============================================
// クラス
// ==============================================
class DirectX11_ShaderManager
{
private:
	// メンバー変数
	TemplateManager<DirectX11_VertexShaderData> m_Vertexs;
	TemplateManager<DirectX11_PixelShaderData> m_Pixels;
	TemplateManager<DirectX11_ComputeShaderData> m_Computes;
	// ファイルパス
	const String& kCompileFilePath;
	const String kCompileFileExt = u8".cso";
	const String kCompileRefExt = u8".ref";

public:
	// コンストラクタ・デストラクタ
	DirectX11_ShaderManager(const String& _path) :kCompileFilePath(_path) {}
	~DirectX11_ShaderManager() = default;

	// シェーダー作成関数
	Handle VertexShaderCreateOnGet (ID3D11Device* _device, const Hashed_String& _name);
	Handle PixelShaderCreateOnGet  (ID3D11Device* _device, const Hashed_String& _name);
	Handle ComputeShaderCreateOnGet(ID3D11Device* _device, const Hashed_String& _name);

	// シェーダー取得関数
	const DirectX11_VertexShaderData* GetVertexShader(const Handle& _handle) {
		return m_Vertexs.GetData(_handle);
	}
	const DirectX11_PixelShaderData* GetPixelShader(const Handle& _handle) {
		return m_Pixels.GetData(_handle);
	}
	const DirectX11_ComputeShaderData* GetComputeShader(const Handle& _handle) {
		return m_Computes.GetData(_handle);
	}

	// シェーダーチェック関数
	bool ExistsVertexShader(const Hashed_String& _name) const {
		return m_Vertexs.Exists(_name);
	}
	bool ExistsPixelShader(const Hashed_String& _name) const {
		return m_Pixels.Exists(_name);
	}
	bool ExistsComputeShader(const Hashed_String& _name) const {
		return m_Computes.Exists(_name);
	}

	// シェーダー削除
	void ReleaseVertexShader(const Handle& _name) {
		m_Vertexs.Remove(_name);
	}
	void ReleasePixelShader(const Handle& _name) {
		m_Pixels.Remove(_name);
	}
	void ReleaseComputeShader(const Handle& _name) {
		m_Computes.Remove(_name);
	}

	// シェーダー全て削除
	void ReleaseAllShader() 
	{
		// 全てクリア
		m_Vertexs.ALLClear();
		m_Pixels.ALLClear();
		m_Computes.ALLClear();
	}
};

