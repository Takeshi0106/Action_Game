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
#include "../../Handle.h"
// データ管理テンプレートヘッダー
#include "../../TemplateManager.h"
// 文字列ヘッダー
#include "../../UTF8_String.h"


// ==============================================
// 構造体宣言
// ==============================================
struct VertexShaderData
{
	// 頂点シェーダー
	Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader = nullptr;
	// 定数バッファハンドル
	std::vector<Handle> constantBufferHandles;
	// 定数バッファレジスタ番号
	std::vector<uint16_t> constantBufferRegisters;
	// 入力レイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout = nullptr;
};

struct PixelShaderData
{
	// ピクセルシェーダー
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader = nullptr;
	// 定数バッファハンドル
	std::vector<Handle> constantBufferHandles;
	// 定数バッファレジスタ番号
	std::vector<uint16_t> constantBufferRegisters;
};

struct ComputeShaderData
{
	// コンピュートシェーダー
	Microsoft::WRL::ComPtr<ID3D11ComputeShader> computeShader = nullptr;
	// 定数バッファハンドル
	std::vector<Handle> constantBufferHandles;
	// 定数バッファレジスタ番号
	std::vector<uint16_t> constantBufferRegisters;
};


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

	// コンパイルファイルパス
	const String& kCompileFilePath;


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
	const Handle VertexShaderCreate (ID3D11Device* _device, const String& _name);
	const Handle PixelShaderCreate  (ID3D11Device* _device, const String& _name);
	const Handle ComputeShaderCreate(ID3D11Device* _device, const String& _name);


	// ------------------------------------------
	// シェーダー取得関数
	// ------------------------------------------
	ID3D11VertexShader* GetVertexShader(const Handle& _handle);
	ID3D11PixelShader* GetPixelShader(const Handle& _handle);
	ID3D11ComputeShader* GetComputeShader(const Handle& _handle);
};

