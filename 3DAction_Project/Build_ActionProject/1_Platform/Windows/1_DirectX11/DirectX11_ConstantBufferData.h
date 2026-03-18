#pragma once

// ========================================
// 【クラス概要】
// DirectX11 用定数バッファデータ
// ========================================


// ========================================
// ヘッダー
// ========================================
// DirectXヘッダー
#include <d3d11.h>
// スマートポインタ
#include <wrl/client.h>
// 整数ヘッダー
#include <cstdint>
// バイナリーデータヘッダー
#include "../../../BinaryView.h"


# if defined(DEBUG) || defined(_DEBUG)
// ========================================
// デバッグ構造体
// ========================================
struct DirectX11_DebugInfo
{
	// バッファサイズ
	uint32_t BufferSize = 0;
	// 使用方法
	D3D11_USAGE Usage = D3D11_USAGE_DEFAULT;
	// CPUアクセスフラグ
	uint32_t CPUAccessFlags = (uint32_t)D3D11_CPU_ACCESS_WRITE;
};
#endif


// ========================================
// クラス
// ========================================
class DirectX11_ConstantBufferData final
{
private:
	// -----------------------------------
	// メンバー変数
	// -----------------------------------
	// 定数バッファ本体
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_ConstantBuffer = nullptr;

	// 更新関数型定義
	using UpdateFunc = bool (DirectX11_ConstantBufferData::*)(
		ID3D11DeviceContext*, const BinaryView&);

	// 定数バッファ更新関数ポインター
	UpdateFunc m_UpdateFunc = nullptr;

#if defined(DEBUG) || defined(_DEBUG)
	// デバッグ情報
	DirectX11_DebugInfo m_DebugInfo{};
#endif


	// -----------------------------------
	// 定数バッファ更新関数群
	// -----------------------------------
	// 毎フレーム更新
	bool Update_DX11ConstantBuffer_Dynamic(
		ID3D11DeviceContext* _context,
		const BinaryView& _binaryView);

	// たまに更新
	bool Update_DX11ConstantBuffer_Default(
		ID3D11DeviceContext* _context,
		const BinaryView& _binaryView);

public:
	// -----------------------------------
	// コンストラクタ・デストラクタ
	// -----------------------------------
	DirectX11_ConstantBufferData() = default;
	~DirectX11_ConstantBufferData() = default;

	// -----------------------------------
	// 定数バッファ作成
	// -----------------------------------
	bool Create_DX11ConstantBuffer(
		ID3D11Device* _device,
		const D3D11_BUFFER_DESC& _desc,
		const D3D11_SUBRESOURCE_DATA* _initData);

	// -----------------------------------
	// 定数バッファ更新
	// -----------------------------------
	bool Update_DX11ConstantBuffer(
		ID3D11DeviceContext* _context,
		const BinaryView& _binaryView);

	// -----------------------------------
	// ゲッター
	// -----------------------------------
	// 定数バッファ取得
	ID3D11Buffer* GetConstantBuffer() const { return m_ConstantBuffer.Get(); }

	// -----------------------------------
	// 削除
	// -----------------------------------
	void Reset() {
		m_ConstantBuffer.Reset();
#if defined(DEBUG) || defined(_DEBUG)
		m_DebugInfo = DirectX11_DebugInfo{};
#endif
	}

	// -----------------------------------
	// デバッグ情報チェック
	// -----------------------------------
#if defined(DEBUG) || defined(_DEBUG)
	bool IsSame(const DirectX11_DebugInfo& _info) const;
#endif
};

