#pragma once

// =============================================================
// 【クラス概要】
// シェーダーの作成、管理、削除をするマネージャー
// シェーダーの作製は初期化時に全て行い、
// 定数バッファマネージャーのポインタを渡し、定数バッファ作成に必要な情報を渡す
// 
// ・デバッグ時
// 　■同じ階層内の.hlslを全て調べて、.cso がない場合　または
// 　　.csoとの最終更新日を比較して、コンパイルが必要かチェックする
// 　■リフレクションして、情報を取得、ログに書出しをする
// 　■リフレクションした情報から、定数バッファを作成、外部ファイルに書出し
// 
// ・リリース時
// 　■リフレクション外部情報を読み込んで、全ての.csoがコンパイルされているかチェック
// 　　されていない場合、hlslファイルから.csoをコンパイルする
// 　■リフレクション外部情報を使用して、定数バッファを作成する
// =============================================================


// ==============================================================
// ヘッダー
// ==============================================================
// シェーダーのデータクラス
#include "ShaderData.h"
// スマートポインターのヘッダー
#include <memory>               // スマートポインター
// 配列のヘッダー
#include <unordered_map>        // ハッシュ値配列
#include <vector>               // 情報を渡す配列
// 基底ヘッダー
#include <string>
// アッセット名ログ出力
#include "AssetLogger.h"


// ==============================================
// 前方宣言
// ==============================================
class ConstantBufferManager; // 定数バッファマネージャー

struct ID3D11Device;         // DirectXのデバイス
struct ID3D11DeviceContext;  // DirectXのコンテキスト

// シェーダに渡す定数バッファや入力レイアウトの情報構造体
class ConstantBufferInfo; // 定数バッファの情報構造体
class InputLayoutInfo;    // 入力レイアウト構造体


// ===================================================================================================
// シェーダーを管理するクラス  （シェイダーのコンパイル、バイナリーファイルのロード、キャッシュ、破棄）
// Debug時にシェーダーをリフレクションして書き出す
// ===================================================================================================
class ShaderManager
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
    std::unordered_map<std::string, std::unique_ptr<VertexShaderData>>  m_Vertexs;  // 頂点シェーダーを入れる配列
    std::unordered_map<std::string, std::unique_ptr<PixelShaderData>>   m_Pixels;   // ピクセルシェーダを入れる配列
    std::unordered_map<std::string, std::unique_ptr<ComputeShaderData>> m_Computes; // コンピュートシェーダーを入れる配列

    // ログ出力
    AssetLogger m_Logger = { "Shader.txt" };

    // 関数
#if defined(DEBUG) || defined(_DEBUG)
    // デバッグ時にこのファイルと同じ階層にある.hlslを探して、コンパイルする必要があるかを確認し、
    // 必要があればコンパイル、無ければバイナリーデータを取得して、メンバー配列に代入する関数
    bool DebugInit(ID3D11Device* device);

#else
    // 全てのシェーダー情報が書かれている外部ファイルから、読込み
    // コンパイルされていなかったら、コンパイルしてメンバー配列に代入する関数
    bool ReleaseInit(ID3D11Device* device);

#endif

    // バイナリーデータを仕分けして、メンバー配列に代入する関数 引き数で拡張子なしの名前を渡す
    bool JudgeBinaryMenber(const std::string shaderName, ID3D11Device* device, void* binary, size_t binarySize,
        const std::vector<ConstantBufferInfo>& CBInfo, const std::vector<InputLayoutInfo>& ILInfo);

public:
    // コンストラクタ
    ShaderManager(const char* CSOPath, const char* hlslPath, const char* infoFaile)
        :kCSOFilePath(CSOPath), kHlslFailePath(hlslPath), kShaderInfoPath(infoFaile) {
    }
    // デストラクタ
    ~ShaderManager() { m_Logger.WriteLog(); }

    // 初期化・後処理
    bool Init(ID3D11Device* device);
    void Uninit();

    // シェーダーのバインド　定数バッファの情報を返す
    const std::vector<ConstantBufferInfo>* BindVertexShader (const std::string& name, ID3D11DeviceContext* context);
    const std::vector<ConstantBufferInfo>* BindPixelShader  (const std::string& name, ID3D11DeviceContext* context);
    const std::vector<ConstantBufferInfo>* BindComputeShader(const std::string& name, ID3D11DeviceContext* context);
};

