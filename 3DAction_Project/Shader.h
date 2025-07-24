#pragma once
#include <string>
#include <d3dcompiler.h>

// ==============================================
// シェーダーの関数群
// ==============================================

// シェーダーをコンパイルしてGPUで使用できるコードに変換する関数　戻り値で成功か失敗を返す
bool CompileShader(const std::wstring FileName, const std::string EntryPoint, const std::string ShaderModel, ID3DBlob** ppBlobOut);
