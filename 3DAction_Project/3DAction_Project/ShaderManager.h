#pragma once
#include "BaseDirectXManager.h" // マネージャークラス

// ==============================================
// 前方宣言
// ==============================================
struct ID3D11Device;     // DirectXのデバイス
class VertexShaderData;  // 頂点シェーダー
class PixelShaderData;   // ピクセルシェーダ
class ComputeShaderData; // コンピュートシェーダ


// ==============================================
// シェーダーの情報
// ==============================================
struct ShaderInfo {
    std::filesystem::path name;  // シェーダーの名前 .hlslを含めてください
    std::string entryPoint;      // シェーダーのエントリーポイント
    std::string shaderTypeModel; // シェーダータイプとモデルを入れる
};


// ===================================================================================================
// シェーダーを管理するクラス  （シェイダーのコンパイル、バイナリーファイルのロード、キャッシュ）
// unordered_map(std::string,shader) 配列で作成する
// ===================================================================================================
class ShaderManager : public BaseDirectXManager
{
private:
    const std::string kHlslFailePath;    // .hlslが入っているフォルダーのパス

    static std::unordered_map<std::string, VertexShaderData>  m_Vertexs;  // 頂点シェーダーを入れる配列
    static std::unordered_map<std::string, PixelShaderData>   m_Pixels;   // ピクセルシェーダを入れる配列
    static std::unordered_map<std::string, ComputeShaderData> m_Computes; // コンピュートシェーダーを入れる配列

    bool OutputCompileShader(const ShaderInfo info); // シェーダーをコンパイルして外部ファイルに書き出す

#if defined(DEBUG) || defined(_DEBUG)
    // デバッグ時のみ有効にする関数

#endif

public:

    // ゲッター
    static VertexShaderData*  GetFindVertexShader (const std::string& name);
    static PixelShaderData*   GetFindPixelShader  (const std::string& name);
    static ComputeShaderData* GetFindComputeShader(const std::string& name);
};

