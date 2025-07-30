#pragma once

// ==============================================================
// Pimpl（ポインタto実装）イディオム で実装し直しても良いかも
// ==============================================================
// 基底クラスヘッダー
#include "BaseDirectXManager.h" // マネージャークラス（基底クラス）
// スマートポインターのヘッダー
#include <memory>               // スマートポインター
// 配列のヘッダー
#include <unordered_map>        // ハッシュ値配列


// ==============================================
// 前方宣言
// ==============================================
struct ID3D11Device;     // DirectXのデバイス
class VertexShaderData;  // 頂点シェーダー
class PixelShaderData;   // ピクセルシェーダ
class ComputeShaderData; // コンピュートシェーダ


// ===================================================================================================
// シェーダーを管理するクラス  （シェイダーのコンパイル、バイナリーファイルのロード、キャッシュ、破棄）
// 初期化時に定数バッファマネージャーのポインターを取得して、リファレクションした定数バッファの内容を渡す
// unordered_map(std::string,shader) 配列で作成すして、検索する
// ===================================================================================================
class ShaderManager : public BaseDirectXManager
{
private:
    // メンバー変数
    const char* kHlslFailePath;    // .hlslが入っているフォルダーのパス

    static std::unordered_map<std::string, std::unique_ptr<VertexShaderData>>  m_Vertexs;  // 頂点シェーダーを入れる配列
    static std::unordered_map<std::string, std::unique_ptr<PixelShaderData>>   m_Pixels;   // ピクセルシェーダを入れる配列
    static std::unordered_map<std::string, std::unique_ptr<ComputeShaderData>> m_Computes; // コンピュートシェーダーを入れる配列

#if defined(DEBUG) || defined(_DEBUG)
    // デバッグ時にこのファイルと同じ階層にある.hlslを探して、コンパイルする必要があるかを確認し、
    // 必要があればコンパイル、無ければバイナリーデータを取得して、メンバー配列に代入する関数
    bool DebugInit(ID3D11Device* device);
#endif

    // 全てのシェーダー情報が書かれている外部ファイルから、読込み
    // コンパイルされていなかったら、コンパイルしてメンバー配列に代入する関数
    bool ReleaseInit(ID3D11Device* device);

    // バイナリーデータを仕分けして、メンバー配列に代入する関数 引き数で拡張子なしの名前を渡す
    bool JudgeBinaryMenber(const std::string shaderName, ID3D11Device* device, void* binary, size_t size);

public:
    ShaderManager(const char* file, const char* assetLog, const char* hlslfaile) // コンストラクタ
        :BaseDirectXManager(file, assetLog), kHlslFailePath(hlslfaile) {}

    bool Init(ID3D11Device* device); // シェーダークラスを作成
    void Uninit();                   // メンバー配列を削除

    // ゲッター
    static VertexShaderData*  GetFindVertexShader (const std::string& name); // 頂点シェーダー　　　の名前を入れて、戻り値で返す
    static PixelShaderData*   GetFindPixelShader  (const std::string& name); // ピクセルシェーダ―　の名前を入れて、戻り値で返す
    static ComputeShaderData* GetFindComputeShader(const std::string& name); // コンピュートシェーダの名前を入れて、戻り値で返す
};

