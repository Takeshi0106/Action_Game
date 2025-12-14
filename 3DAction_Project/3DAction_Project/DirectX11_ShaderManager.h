#pragma once

// ==============================================
// 【クラス概要】
// DirectX11用シェーダーマネージャー
// APIObject を管理するマネージャー
// ==============================================


// ==============================================
// ヘッダー
// ==============================================
// DirectXヘッダー
#include <d3d11.h>
// スマートポインタ
#include <wrl/client.h>
// ハンドルヘッダー
#include "Handle.h"
// データ管理テンプレートヘッダー
#include "TemplateManager.h"


// ==============================================
// クラス
// ==============================================
class DirectX11_ShaderManager
{
private:
	// ------------------------------------------
	// メンバー変数
	// ------------------------------------------
	// シェーダー管理
	TemplateManager<Microsoft::WRL::ComPtr<ID3D11VertexShader>> m_Vertexs;
	TemplateManager<Microsoft::WRL::ComPtr<ID3D11PixelShader>> m_Pixels;
	TemplateManager<Microsoft::WRL::ComPtr<ID3D11ComputeShader>> m_Computes;


public:
	// ------------------------------------------
	// コンストラクタ・デストラクタ
	// ------------------------------------------
	DirectX11_ShaderManager() = default;
	~DirectX11_ShaderManager() = default;


	// ------------------------------------------
	// シェーダー作成関数
	// ------------------------------------------
	const Handle VertexShaderCreate(ID3D11Device* _device, const void* _bytecode, size_t _size, const char* _name);
	const Handle PixelShaderCreate(ID3D11Device* _device, const void* _bytecode, size_t _size, const char* _name);
	const Handle ComputeShaderCreate(ID3D11Device* _device, const void* _bytecode, size_t _size, const char* _name);


	// ------------------------------------------
	// シェーダー取得関数
	// ------------------------------------------
	ID3D11VertexShader* GetVertexShader(const Handle& _handle);
	ID3D11PixelShader* GetPixelShader(const Handle& _handle);
	ID3D11ComputeShader* GetComputeShader(const Handle& _handle);
};

