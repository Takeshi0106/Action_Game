#pragma once
// ==============================================================
// ＊このヘッダーを絶対にヘッダーにインクルードしないでください
// D3D11ヘッダーが含まれています
// ==============================================================
// DirectX用
#include <d3d11.h>       // DirectXのAPI
#include <wrl/client.h>  // マイクロソフトが提供するスマートポインタ
// 名前などのデバッグ情報取得用
#include <string>        // 名前など
// シェーダーに持たせる情報
#include "ConstantBufferInfo.h" // 定数バッファの情報
#include "InputLayoutInfo.h"    // 入力レイアウトの情報
#include <vector>          // 配列


// ====================================================================
// シェーダークラス
// シェイダーの保持と、利用の責任を持つ
// ====================================================================

// 共通する変数や関数などをまとめるためのクラス 
// 使用できないようにコンストラクタをプロテクトに入れてインスタンス化できないようにしています。
class BaseShaderData {
protected:
	std::string           m_Name;            // シェーダーの名前　＊ログ出力用
	std::string           m_EntryPoint;      // シェーダーのエントリーポイント
	std::string           m_ShaderTypeModel; // シェーダータイプとモデルを入れる

	std::vector<ConstantBufferInfo> CBInfo;  // シェーダーと紐づく定数バッファを代入する

	// コンストラクタ
	BaseShaderData(std::string name, std::string entry, std::string type)
		:m_Name(name), m_EntryPoint(entry), m_ShaderTypeModel(type) {
	}
	virtual ~BaseShaderData() = default;

public:
	// ゲッター
	std::string GetName()       const { return m_Name; }
	std::string GetEntryPoint() const { return m_EntryPoint; }
	std::string GetTypeModel()  const { return m_ShaderTypeModel; }
	// 定数バッファ情報
	const std::vector<ConstantBufferInfo>& GetCBInfo() const { return CBInfo; }
};


// 頂点シェーダクラス
class  VertexShaderData : public BaseShaderData {
private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_VertexShader; // 頂点シェーダー

	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_ILayout; // シェーダーの入力レイアウト情報を入れる配列

public:
	VertexShaderData(std::string name, std::string entry, std::string type) :BaseShaderData(name, entry, type) {}   // コンストラクタ
	~VertexShaderData() = default;                                                                                   // デストラクタ

	bool CreateShader(ID3D11Device* device, void* binary, size_t size,
	const std::vector<ConstantBufferInfo>& _CBInfo, const std::vector<InputLayoutInfo>& _ILInfo); // シェーダー作成

	// ゲッター
	ID3D11VertexShader* GetShader() { return m_VertexShader.Get(); }  // シェーダーのゲッター
	// 入力レイアウト情報（頂点シェーダーのみ）
	const Microsoft::WRL::ComPtr<ID3D11InputLayout>& GetILInfo() const { return m_ILayout; }
};


// ピクセルシェーダクラス
class  PixelShaderData : public BaseShaderData {
private:
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_PixelShader; // ピクセルシェーダー

public:
	PixelShaderData(std::string name, std::string entry, std::string type) :BaseShaderData(name, entry, type) {}  // コンストラクタ
	~PixelShaderData() = default;                                                                               // デストラクタ

	bool CreateShader(ID3D11Device* device, void* binary, size_t size,
		const std::vector<ConstantBufferInfo>& _CBInfo); // シェーダー作成

	ID3D11PixelShader* GetShader() { return m_PixelShader.Get(); }  // シェーダーのゲッター
};


// コンピュートシェーダークラス
class ComputeShaderData : public BaseShaderData {
private:
	Microsoft::WRL::ComPtr<ID3D11ComputeShader> m_ComputeShader; // コンピュートシェーダー

public:
	ComputeShaderData(std::string name, std::string entry, std::string type) :BaseShaderData(name, entry, type) {} // コンストラクタ
	~ComputeShaderData() = default;                                                                             // デストラクタ

	bool CreateShader(ID3D11Device* device, void* binary, size_t size,
		const std::vector<ConstantBufferInfo>& _CBInfo); // シェーダー作成

	ID3D11ComputeShader* GetShader() { return m_ComputeShader.Get(); } // シェーダーのゲッター
};