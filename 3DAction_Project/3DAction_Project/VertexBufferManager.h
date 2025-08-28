#pragma once

// =====================================
// 【クラス概要】
// 頂点バッファマネージャー
// 
// 作成、管理、削除を担当
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


// ======================================
// 前方宣言
// ======================================
struct ID3D11Device;        // DirectXのデバイス
struct ID3D11DeviceContext; // DirectXのデバイスコンテキスト
struct ID3D11Buffer;        // 定数バッファ
struct VertexBufferData;    // 頂点バッファ構造体 (入力レイアウトを持つ)


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
	bool CreateVertexBuffer(const std::string& name, size_t size, int slot, ID3D11Device* device);
	// 頂点バッファを探して、戻り値で返す
	ID3D11Buffer* GetFindVertexBuffer(const std::string& name);

	// 後処理
	void ReleaseAllVertexBuffers();

};

