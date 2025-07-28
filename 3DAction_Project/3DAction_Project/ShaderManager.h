#pragma once
#include "BaseDirectXManager.h" // マネージャークラス（基底クラス）
#include <memory>               // スマートポインター
#include <unordered_map>        // ハッシュ値配列


// ==============================================
// 前方宣言
// ==============================================
struct ID3D11Device;     // DirectXのデバイス
class VertexShaderData;  // 頂点シェーダー
class PixelShaderData;   // ピクセルシェーダ
class ComputeShaderData; // コンピュートシェーダ


// ===================================================================================================
// シェーダーを管理するクラス  （シェイダーのコンパイル、バイナリーファイルのロード、キャッシュ）
// unordered_map(std::string,shader) 配列で作成する
// ===================================================================================================
class ShaderManager : public BaseDirectXManager
{
private:
    // メンバー変数
    const std::string kHlslFailePath;    // .hlslが入っているフォルダーのパス

    static std::unordered_map<std::string, std::unique_ptr<VertexShaderData>>  m_Vertexs;  // 頂点シェーダーを入れる配列
    static std::unordered_map<std::string, std::unique_ptr<PixelShaderData>>   m_Pixels;   // ピクセルシェーダを入れる配列
    static std::unordered_map<std::string, std::unique_ptr<ComputeShaderData>> m_Computes; // コンピュートシェーダーを入れる配列

#if defined(DEBUG) || defined(_DEBUG)
    // デバッグ時のみ有効にする関数
    bool DebugInit(ID3D11Device* device); // 同じ階層にある.hlslを全てコンパイルして出力する関数

#endif

public:
    ShaderManager(std::filesystem::path file, std::filesystem::path assetLog)
        :BaseDirectXManager(file, assetLog) {
    } // コンストラクタ

    bool Init(ID3D11Device* device); // シェイダーを保存するクラス
    void Uninit();

    // ゲッター
    static VertexShaderData* GetFindVertexShader(const std::string& name);
    static PixelShaderData* GetFindPixelShader(const std::string& name);
    static ComputeShaderData* GetFindComputeShader(const std::string& name);
};

