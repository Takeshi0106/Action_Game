#pragma once
// ===========================================
// 【クラス概要】
// 定数バッファの作成、管理、削除を行うマネージャー
// 
// 定数バッファ、頂点バッファを管理
// ===========================================


// =======================================
// ヘッダー
// =======================================
// 基底クラスヘッダー
#include "BaseDirectXManager.h"
// 標準ヘッダー
#include <string>
// スマートポインターヘッダー
#include <memory> // スマートポインター
// 配列のヘッダー
#include <unordered_map> // ハッシュ値検索


// ======================================
// 前方宣言
// ======================================
struct ID3D11Device;        // DirectXのデバイス
struct ID3D11DeviceContext; // DirectXのデバイスコンテキスト
struct ID3D11Buffer;        // 定数バッファ
struct ConstantBufferData;  // 定数バッファ構造体 (定数バッファとサイズを持つ)


// ======================================
// 定数バッファマネージャ 
// ファイルパスに定数バッファの情報が入っているファイルのパスを入れる
// シェーダ―マネージャーにポインターを渡し
// 定数バッファを作成する（単一結合）
// ======================================
class ConstantBufferManager : public BaseDirectXManager
{
private:
	// 定数バッファメンバー配列
	static std::unordered_map<std::string, std::unique_ptr<ConstantBufferData>> m_ConstantBuffers;

public:
	// コンストラクタ
	ConstantBufferManager(const char* assetLog) : BaseDirectXManager(assetLog) {}
	// デストラクタ
	~ConstantBufferManager() = default;

	// 定数バッファ作成
	bool CreateConstantBuffer(const std::string& name, size_t size, int slot, ID3D11Device* device);
	// 定数バッファを探して、戻り値で返す
	ID3D11Buffer* GetFindConstantBuffer(const std::string& name);

	// 後処理
	void ReleaseAllConstantBuffers();
};

