#pragma once
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
// シェーダ―マネージャーにポインターを渡し
// 定数バッファを作成する（単一結合）
// ======================================
class ConstantBufferManager
{
private:
	static std::unordered_map<std::string, std::unique_ptr<ConstantBufferData>> m_ConstantBuffer; // 定数バッファメンバー配列

public:
	bool CreateConstantBuffer(const std::string& name, size_t size, int slot, ID3D11Device* device); // 定数バッファ作成
	bool UpdateConstantBuffer(const std::string& name, const void* data, size_t dataSize, ID3D11DeviceContext* context); // 定数バッファを更新
	ID3D11Buffer* GetFindConstantBuffer(const std::string& name); // 定数バッファを探して、戻り値で返す

	bool BindVS(const std::string& name, ID3D11DeviceContext* context);
	bool BindPS(const std::string& name, ID3D11DeviceContext* context);
	bool BindCS(const std::string& name, ID3D11DeviceContext* context);
};

