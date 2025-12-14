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
// シェーダーに持たせる情報
 // 定数バッファの情報
#include "ConstantBufferInfo.h"
// 入力レイアウトの情報
#include "InputLayoutInfo.h"
// 配列
#include <vector> 
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
class BaseShaderData 
{
protected:
	// シェーダーと紐づく定数バッファ情報を代入する
	std::vector<ConstantBufferInfo> CBInfo;

	// --------------------------------
	// コンストラクタ・デストラクタ
	// --------------------------------
	BaseShaderData() = default;
	virtual ~BaseShaderData() = default;

public:
	// --------------------------------
	// 定数バッファ情報
	// --------------------------------
	const std::vector<ConstantBufferInfo>* GetCBInfo() const { return &CBInfo; }
};


// ========================================
// 頂点シェーダクラス
// ========================================
class  VertexShaderData final : public BaseShaderData 
{
private:
	// --------------------------------
	// メンバー変数
	// --------------------------------
	// 頂点シェーダー
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_VertexShader;
	// シェーダーの入力レイアウト情報を入れる
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_ILayout;

public:
	// --------------------------------
	// コンストラクタ・デストラクタ
	// --------------------------------
	VertexShaderData() = default;
	~VertexShaderData() override final = default;

	// --------------------------------
	// メンバー関数
	// --------------------------------
	// シェーダー作成
	bool CreateVertexShader(ID3D11Device* device, void* binary, size_t size,
	const std::vector<ConstantBufferInfo>& _CBInfo, const std::vector<InputLayoutInfo>& _ILInfo);
	// バインド
	void BindVertexShader(ID3D11DeviceContext* context);
};


// ========================================
// ピクセルシェーダクラス
// ========================================
class  PixelShaderData final : public BaseShaderData 
{
private:
	// ---------------------------------
	// ピクセルシェーダー
	// ---------------------------------
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_PixelShader;

public:
	// ---------------------------------
	// コンストラクタ・デストラクタ
	// ---------------------------------
	PixelShaderData() = default; 
	~PixelShaderData() override final = default;

	// ---------------------------------
	// メンバー変数
	// ---------------------------------
	// シェーダー作成
	bool CreatePixelShader(ID3D11Device* device, void* binary, size_t size,
		const std::vector<ConstantBufferInfo>& _CBInfo);
	// バインド
	void BindPixelShader(ID3D11DeviceContext* context);
};


// コンピュートシェーダークラス
class ComputeShaderData final : public BaseShaderData 
{
private:
	// ---------------------------------
	// コンピュートシェーダー
	// ---------------------------------
	Microsoft::WRL::ComPtr<ID3D11ComputeShader> m_ComputeShader;

public:
	// ---------------------------------
	// コンストラクタ・デストラクタ
	// ---------------------------------
	ComputeShaderData() = default; 
	~ComputeShaderData() override final = default;

	// ---------------------------------
	// メンバー変数
	// シェーダー作成
	bool CreateComputeShader(ID3D11Device* device, void* binary, size_t size,
		const std::vector<ConstantBufferInfo>& _CBInfo);
	// バインド
	void BindComputeShader(ID3D11DeviceContext* context);
};
