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
// DirectXヘッダー
#include <d3d11.h>        // DirectXのAPIヘッダー
#include <wrl/client.h>   // スマートポインター
// 定数バッファデータ
#include "ConstantBufferData.h"
// 標準ヘッダー
#include <string>
// スマートポインターヘッダー
#include <memory> // スマートポインター
// 配列のヘッダー
#include <unordered_map> // ハッシュ値検索
// バッファ設定ヘッダー
#include "GraphicsEnums.h"
// バインド情報ヘッダー
#include "ConstantBufferInfo.h"
// バインド情報を入れる配列ヘッダー
#include <vector>
// 使用できるシェーダーの種類を入れる
#include "UseShaderType.h"
// 外部ファイルにアセット名ログ出量用
#include "AssetLogger.h"


// ======================================
// 定数バッファマネージャ 
// ファイルパスに定数バッファの情報が入っているファイルのパスを入れる
// シェーダ―マネージャーにポインターを渡し
// 定数バッファを作成する（単一結合）
// ======================================
class ConstantBufferManager
{
private:
	// 定数バッファメンバー配列
	std::unordered_map<std::string, std::unique_ptr<ConstantBufferData>> m_ConstantBuffers;
	AssetLogger m_Logger = { "ConstantBuffers.txt" };

public:
	// コンストラクタ
	ConstantBufferManager() = default;
	// デストラクタ
	~ConstantBufferManager() { m_Logger.WriteLog(); }

	// 定数バッファ作成
	bool CreateConstantBuffer(const std::string constantName,
		ID3D11Device* device,
		const void* data,
		size_t size,
		BufferUsage usage = BufferUsage::Dynamic,
		CPUAccess access = CPUAccess::Write);

	// 定数バッファ更新
	bool UpdateConstantBuffer(const std::string& name, ID3D11DeviceContext* context, const void* data, int size);

	// 定数バッファを探して、バインド
	bool BindConstantBuffer(const std::vector<ConstantBufferInfo>* cbInfo, ID3D11DeviceContext* context, SETSHADERTYPE type);

	// 後処理
	void ReleaseAllConstantBuffers();
};

