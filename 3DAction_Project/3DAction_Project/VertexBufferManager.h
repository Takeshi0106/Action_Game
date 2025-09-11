#pragma once

// =====================================
// 【クラス概要】
// 頂点バッファマネージャー
// 
// 作成、管理、削除を担当
// VertexBufferDataに重たいヘッダーがあります
// =====================================


// ======================================
// ヘッダー
// ======================================
// 必須ヘッダー
#include "BaseDirectXManager.h"
// スマートポインターヘッダー
#include <memory>
// 配列のヘッダー
#include <unordered_map>
// バッファ設定ヘッダー
#include "BufferSetting.h"


// ======================================
// 前方宣言
// ======================================
struct ID3D11Device;        // DirectXのデバイス
struct ID3D11DeviceContext; // DirectXのデバイスコンテキスト
struct ID3D11Buffer;        // 定数バッファ
class VertexBufferData;    // 頂点バッファデータクラス


// ======================================
// クラス
// ======================================
class VertexBufferManager : public BaseDirectXManager
{
private:
	// 頂点バッファメンバー配列
	static std::unordered_map<std::string, std::unique_ptr<VertexBufferData>> m_VertexBuffers;

public:
	// コンストラクタ
	VertexBufferManager(const char* assetLog) : BaseDirectXManager(assetLog) {}
	// デストラクタ
	~VertexBufferManager() = default;

	// 頂点バッファ作成
	bool CreateVertexBuffer(
		const std::string& name,
		ID3D11Device* device,
		const void* vertices,
		int vertexCount,
		int vertexMaxCount,
		size_t stride,
		PrimitiveType type = PrimitiveType::TriangleStrip,
		BufferUsage usage = BufferUsage::Dynamic,
		CPUAccess access = CPUAccess::Write
	);

	// 頂点バッファを探して、戻り値で返す
	VertexBufferData* GetFindVertexData(const std::string& name) const;
	// 頂点バッファがあるかのチェック
	bool Exists(const std::string& name) const;

	// 後処理
	void ReleaseAllVertexBuffers();

};

