#pragma once
// ==============================================================
// ＊このヘッダーを絶対にヘッダーにインクルードしないでください
// D3D11ヘッダーが含まれています
// ==============================================================
#include <d3d11.h>
#include <wrl/client.h>  // マイクロソフトが提供するスマートポインタ
// 名前などのデバッグ情報取得用
#include <string>        // 名前など
// シェーダーに持たせる情報
#include "ConstantBufferInfo.h" // 定数バッファの情報
#include "InputLayoutInfo.h"    // 入力レイアウトの情報
#include <vector>          // 配列
// std::unique_ptrを使用するため
#include <memory>


// ===============================================================
// 前方宣言
// ===============================================================
// D3D11 の前方宣言
struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11VertexShader;
struct ID3D11PixelShader;
struct ID3D11ComputeShader;
struct ID3D11InputLayout;

// PImplイディオム
struct VertexDataImpl;
struct PixelDataImpl;
struct ComputeDataImpl;


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
	std::unique_ptr<VertexDataImpl> m_VertexData;

public:
	// コンストラクタ・デストラクタ
	VertexShaderData();
	~VertexShaderData();

	// シェーダー作成
	bool CreateShader(ID3D11Device* device, void* binary, size_t size,
	const std::vector<ConstantBufferInfo>& _CBInfo, const std::vector<InputLayoutInfo>& _ILInfo);

	// シェーダーバインド
	void VertexShaderBind(ID3D11DeviceContext* contex);
};


// ピクセルシェーダクラス
class  PixelShaderData : public BaseShaderData {
private:
	std::unique_ptr<PixelDataImpl> m_PixelData;

public:
	PixelShaderData();  // コンストラクタ
	~PixelShaderData(); // デストラクタ

	// シェーダー作成
	bool CreateShader(ID3D11Device* device, void* binary, size_t size,
		const std::vector<ConstantBufferInfo>& _CBInfo);

	// バインド
	void PixelShaderBind(ID3D11DeviceContext* context);
};


// コンピュートシェーダークラス
class ComputeShaderData : public BaseShaderData {
private:
	std::unique_ptr<ComputeDataImpl> m_ComputeData;

public:
	ComputeShaderData(); // コンストラクタ
	~ComputeShaderData(); // デストラクタ

	// シェーダー作成
	bool CreateShader(ID3D11Device* device, void* binary, size_t size,
		const std::vector<ConstantBufferInfo>& _CBInfo);

	// コンピュートシェーダーをセット
	void ComputeShaderBind(ID3D11DeviceContext* context);
};