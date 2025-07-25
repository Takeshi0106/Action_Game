#pragma once
#include <string>
#include <d3dcompiler.h>

// シェーダーの関数　
// 今は　Shader Model 5.0のみ使用可能

// ==============================================
// シェーダーの種類
// ==============================================
enum ShaderType {
    VERTEX_5_0 = 0,
    PIXEL_5_0,
    GEOMETRY_5_0,
    HULL_5_0,
    DOMAIN_5_0,
    COMPUTE_5_0
};


// ==============================================
// シェーダーの関数群
// ==============================================

// シェーダーをコンパイルしてGPUで使用できるコードに変換する関数　戻り値で成功か失敗を返す
// 今は.csoを書き出す機能を作成していません
bool CompileShader(const std::wstring FileName, const std::string EntryPoint, const ShaderType type, ID3DBlob* ppBlobOut);

