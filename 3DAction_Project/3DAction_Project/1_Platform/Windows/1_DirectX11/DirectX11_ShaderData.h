#pragma once

// ==============================================
// 【クラス概要】
// DirectX11 用シェーダーデータ
// ==============================================


// ==============================================
// ヘッダー
// ==============================================
// DirectXヘッダー
#include <d3d11.h>
// スマートポインタ
#include <wrl/client.h>
// 配列ヘッダー
#include <vector>
// 整数ヘッダー
#include <cstdint>
// バイナリーデータ
#include "../../../BinaryView.h"
// 定数バッファ情報構造体ヘッダー
#include "../../../ShaderConstantInfo.h"


// ==============================================
// 頂点シェーダーデータ
// ==============================================
class DirectX11_VertexShaderData final
{
private:
	// ---------------------------------------------
	// 実態
	// ---------------------------------------------
	// 頂点シェーダー
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_VertexShader = nullptr;
	// 入力レイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_InputLayout = nullptr;

	// ---------------------------------------------
	// 定数バッファ情報
	// ---------------------------------------------
	std::vector<ShaderConstantInfo> m_ShaderConstantInfo;


public:
	// ---------------------------------------------
	// コンストラクタ・デストラクタ
	// ---------------------------------------------
	DirectX11_VertexShaderData() = default;
	~DirectX11_VertexShaderData() = default;

	// ---------------------------------------------
	// シェーダー作成
	// ---------------------------------------------
	bool Create_DX11VertexShader(
		ID3D11Device* _device,
		const BinaryView& _shaderData,
		const std::vector<D3D11_INPUT_ELEMENT_DESC>& _inputDescs,
		const std::vector<ShaderConstantInfo>& _constantInfo);

	// ---------------------------------------------
	// ゲッター
	// ---------------------------------------------
	// 頂点シェーダー取得
	ID3D11VertexShader* GetVertexShader() const { return m_VertexShader.Get(); }
	// 入力レイアウト取得
	ID3D11InputLayout* GetInputLayout() const { return m_InputLayout.Get(); }
	// 定数バッファ情報取得
	const std::vector<ShaderConstantInfo>& GetConstantInfo() const { return m_ShaderConstantInfo; }

	// ---------------------------------------------
	// 削除
	// ---------------------------------------------
	void Release() {
		m_VertexShader.Reset();
		m_InputLayout.Reset();
		m_ShaderConstantInfo.clear();
	}
};


// ==============================================
// ピクセルシェーダーデータ
// ==============================================
class DirectX11_PixelShaderData final
{
private:
	// ---------------------------------------------
	// 実態
	// ---------------------------------------------
	// ピクセルシェーダー
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_PixelShader = nullptr;

	// ---------------------------------------------
	// 定数バッファ情報
	// ---------------------------------------------
	std::vector<ShaderConstantInfo> m_ShaderConstantInfo;


public:
	// ---------------------------------------------
	// コンストラクタ・デストラクタ
	// ---------------------------------------------
	DirectX11_PixelShaderData() = default;
	~DirectX11_PixelShaderData() = default;

	// ---------------------------------------------
	// シェーダー作成
	// ---------------------------------------------
	bool Create_DX11PixelShader(
		ID3D11Device* _device,
		const BinaryView& _shaderData,
		const std::vector<ShaderConstantInfo>& _constantInfo);

	// ---------------------------------------------
	// ゲッター
	// ---------------------------------------------
	// ピクセルシェーダー取得
	ID3D11PixelShader* GetPixelShader() const { return m_PixelShader.Get(); }
	// 定数バッファ情報取得
	const std::vector<ShaderConstantInfo>& GetConstantInfo() const { return m_ShaderConstantInfo; }

	// ---------------------------------------------
	// 削除
	// ---------------------------------------------
	void Release() {
		m_PixelShader.Reset();
		m_ShaderConstantInfo.clear();
	}
};


// ==============================================
// コンピュートシェーダーデータ
// ==============================================
class DirectX11_ComputeShaderData final
{
private:
	// ---------------------------------------------
	// 実態
	// ---------------------------------------------
	// コンピュートシェーダー
	Microsoft::WRL::ComPtr<ID3D11ComputeShader> m_ComputeShader = nullptr;

	// ---------------------------------------------
	// 定数バッファ情報
	// ---------------------------------------------
	std::vector<ShaderConstantInfo> m_ShaderConstantInfo;

public:
	// ---------------------------------------------
	// コンストラクタ・デストラクタ
	// ---------------------------------------------
	DirectX11_ComputeShaderData() = default;
	~DirectX11_ComputeShaderData() = default;

	// ---------------------------------------------
	// シェーダー作成
	// ---------------------------------------------
	bool Create_DX11ComputeShader(
		ID3D11Device* _device,
		const BinaryView& _shaderData,
		const std::vector<ShaderConstantInfo>& _constantInfo);

	// ---------------------------------------------
	// ゲッター
	// ---------------------------------------------
	// コンピュートシェーダー取得
	ID3D11ComputeShader* GetComputeShader() const { return m_ComputeShader.Get(); }
	// 定数バッファ情報取得
	const std::vector<ShaderConstantInfo>& GetConstantInfo() const { return m_ShaderConstantInfo; }

	// ---------------------------------------------
	// 削除
	// ---------------------------------------------
	void Release() {
		m_ComputeShader.Reset();
		m_ShaderConstantInfo.clear();
	}
};
