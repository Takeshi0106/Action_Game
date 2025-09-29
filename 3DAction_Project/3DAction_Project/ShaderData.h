#pragma once

// ==============================================================
// シェーダークラス
// 
// 【責任】
// シェーダーの作成、シェーダーとセットで使用する定数バッファの名前、バインド番号などを保持
// ＊バインドなどはDrawManageで行う
// ==============================================================


// ========================================
// ヘッダー
// ========================================
// 名前などのデバッグ情報取得用
#include <string>        // 名前など
// シェーダーに持たせる情報
#include "ConstantBufferInfo.h" // 定数バッファの情報
#include "InputLayoutInfo.h"    // 入力レイアウトの情報
#include <vector>          // 配列
// DirectXヘッダー
#include <d3d11.h>
// マイクロソフトが提供するスマートポインタ
#include <wrl/client.h> 


// ====================================================================
// シェーダークラス
// シェイダーの保持と、利用の責任を持つ
// ====================================================================
// 共通する変数や関数などをまとめるためのクラス 
// 使用できないようにコンストラクタをプロテクトに入れてインスタンス化できないようにしています。
class BaseShaderData {
protected:
	std::vector<ConstantBufferInfo> CBInfo;  // シェーダーと紐づく定数バッファ情報を代入する

	// コンストラクタ
	BaseShaderData() = default;
	virtual ~BaseShaderData() = default;

public:
	// 定数バッファ情報
	const std::vector<ConstantBufferInfo>& GetCBInfo() const { return CBInfo; }
};


// 頂点シェーダクラス
class  VertexShaderData : public BaseShaderData {
private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_VertexShader; // 頂点シェーダー
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_ILayout; // シェーダーの入力レイアウト情報を入れる

public:
	// コンストラクタ・デストラクタ
	VertexShaderData() = default;
	~VertexShaderData() = default;

	// シェーダー作成
	bool CreateShader(ID3D11Device* device, void* binary, size_t size,
	const std::vector<ConstantBufferInfo>& _CBInfo, const std::vector<InputLayoutInfo>& _ILInfo);

	// ゲッター
	ID3D11VertexShader* GetVertexShader() { return m_VertexShader.Get(); }
	ID3D11InputLayout* GetInputLayout() { return m_ILayout.Get(); }
};


// ピクセルシェーダクラス
class  PixelShaderData : public BaseShaderData {
private:
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_PixelShader; // ピクセルシェーダー

public:
	PixelShaderData() = default;  // コンストラクタ
	~PixelShaderData() = default; // デストラクタ

	// シェーダー作成
	bool CreateShader(ID3D11Device* device, void* binary, size_t size,
		const std::vector<ConstantBufferInfo>& _CBInfo);

	// ゲッター
	ID3D11PixelShader* GetPixelShader() { return m_PixelShader.Get(); }
};


// コンピュートシェーダークラス
class ComputeShaderData : public BaseShaderData {
private:
	Microsoft::WRL::ComPtr<ID3D11ComputeShader> m_ComputeShader; // コンピュートシェーダー

public:
	ComputeShaderData() = default; // コンストラクタ
	~ComputeShaderData() = default; // デストラクタ

	// シェーダー作成
	bool CreateShader(ID3D11Device* device, void* binary, size_t size,
		const std::vector<ConstantBufferInfo>& _CBInfo);

	// ゲッター
	ID3D11ComputeShader* GetComputeShader() { return m_ComputeShader.Get(); }
};