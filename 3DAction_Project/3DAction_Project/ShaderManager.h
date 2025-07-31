#pragma once

// =============================================================
// コンストラクタの引数情報
// デバッグ
// １　デバッグコンパイルファイルパス
// ２　シェーダーの名前ログ
// ３　HLSLパス　デバッグ時無効
// ４　リフレクション書出し
// 
// リリース
// １　リリースコンパイルファイルパス
// ２　HLSLパス　HLSLがおいてあるファイル
// ３　リフレクション読込み
// =============================================================


/*
ファイル分け

1. コンパイル
2. リフレクション
3. Managerに分ける
*/


// ==============================================================
// Pimpl（ポインタto実装）イディオム で実装し直しても良いかも
// ==============================================================
// 基底クラスヘッダー
#include "BaseDirectXManager.h" // マネージャークラス（基底クラス）
// スマートポインターのヘッダー
#include <memory>               // スマートポインター
// 配列のヘッダー
#include <unordered_map>        // ハッシュ値配列
// 基底ヘッダー
#include <string>

// ==============================================
// 前方宣言
// ==============================================
class ConstantBufferManager; // 定数バッファマネージャー

struct ID3D11Device;         // DirectXのデバイス
struct ID3D11DeviceContext;  // DirectXのコンテキスト

// メンバー配列に入れるクラス
class VertexShaderData;  // 頂点シェーダー
class PixelShaderData;   // ピクセルシェーダ
class ComputeShaderData; // コンピュートシェーダ


// ===================================================================================================
// シェーダーを管理するクラス  （シェイダーのコンパイル、バイナリーファイルのロード、キャッシュ、破棄）
// Debug時にシェーダーをリフレクションして書き出す
// ===================================================================================================
class ShaderManager : public BaseDirectXManager
{
private:
    // ---------------------------------
    // メンバー変数
    // ---------------------------------

    // ファイルパス
    const char* kCSOFilePath;     // .CSOを参照しに行くパス
    const char* kHlslFailePath;  // .hlslが入っているフォルダーのパス
    const char* kShaderInfoPath; // シェーダーや定数バッファの情報が入っている

    // シェーダー保存配列
    static std::unordered_map<std::string, std::unique_ptr<VertexShaderData>>  m_Vertexs;  // 頂点シェーダーを入れる配列
    static std::unordered_map<std::string, std::unique_ptr<PixelShaderData>>   m_Pixels;   // ピクセルシェーダを入れる配列
    static std::unordered_map<std::string, std::unique_ptr<ComputeShaderData>> m_Computes; // コンピュートシェーダーを入れる配列


    // 関数
#if defined(DEBUG) || defined(_DEBUG)
    // デバッグ時にこのファイルと同じ階層にある.hlslを探して、コンパイルする必要があるかを確認し、
    // 必要があればコンパイル、無ければバイナリーデータを取得して、メンバー配列に代入する関数
    bool DebugInit(ID3D11Device* device, ConstantBufferManager& CBManager);

#else
    // 全てのシェーダー情報が書かれている外部ファイルから、読込み
    // コンパイルされていなかったら、コンパイルしてメンバー配列に代入する関数
    bool ReleaseInit(ID3D11Device* device, ConstantBufferManager& CBManager);

#endif

    // バイナリーデータを仕分けして、メンバー配列に代入する関数 引き数で拡張子なしの名前を渡す
    bool JudgeBinaryMenber(const std::string shaderName, ID3D11Device* device, void* binary, size_t binarySize);

public:
#if defined(DEBUG) || defined(_DEBUG)
    // デバッグ時のコンストラクタ　
    // リリース時との違い　使用したオブジェクトパスを出力できるようにしている
    ShaderManager(const char* assetLog, const char* CSOPath, const char* hlslPath, const char* infoFaile)
        :BaseDirectXManager(assetLog), kCSOFilePath(CSOPath), kHlslFailePath(hlslPath), kShaderInfoPath(infoFaile) {
    }

#else
    // リリース時のコンストラクタ
    ShaderManager(const char* CSOPath, const char* hlslfaile, const char* Infofaile)
        : kCSOFilePath(CSOPath), kHlslFailePath(hlslfaile), kShaderInfoPath(Infofaile) {
    }

#endif

    // 初期化・後処理
    bool Init(ID3D11Device* device, ConstantBufferManager& CBManager);
    void Uninit();

    // シェーダーのゲッター  名前を入れて、返す
    static VertexShaderData*  GetFindVertexShader (const std::string& name);
    static PixelShaderData*   GetFindPixelShader  (const std::string& name);
    static ComputeShaderData* GetFindComputeShader(const std::string& name);

    // シェーダーをセットする関数
    void BindVertexShaderSet(const std::string name, ID3D11DeviceContext* context);
    void BindPixelShaderSet(const std::string name, ID3D11DeviceContext* context);
    void BindComputeShaderSet(const std::string name, ID3D11DeviceContext* context);
};

