
// ==============================================
// ヘッダー
// ==============================================
// 必須ヘッダー
#include "Windows_SPIRVCompileModule.h"
// ウィンドウズヘッダー
#include <Windows.h>
// スマートポインター
#include <wrl/client.h>
// ファイルシステムヘッダー
#include <filesystem>
// シェーダー便利関数ヘッダー
#include "../../../ShaderUtility.h"
// レポートログ出力
#include "../../../ReportMessage.h"


// ==============================================
// HLSL を SPIR-V に変換する
// ==============================================
bool Windows_SPIRV_CompileModule::SPIRVCompile(const String& _hlslName)
{
    // dxc.exe のパス
    std::filesystem::path dxcPath = "C:/0_Spirv/dxc/dxc.exe";

	dxcPath = dxcPath.generic_string();

	// .hlslパス取得
    std::filesystem::path shaderPath = std::filesystem::absolute(
		std::filesystem::path(kHlslFolderPath.GetU8String()) /
        (_hlslName.GetU8String() + kHlslExtension.GetU8String()));

    shaderPath = shaderPath.generic_string();

    // 出力ファイルパス
    std::filesystem::path outputPath = std::filesystem::absolute(
        std::filesystem::path(kSPIRVFolderPath.GetU8String()) /
        (shaderPath.stem().u8string() + kSPIRVExtension.GetU8String()));

    // 区切り文字を統一
    outputPath = outputPath.generic_string();

    // シェーダータイプを取得
    SETSHADERTYPE type = ShaderUtility::GetShaderTypeFromFileName(shaderPath.filename().u8string());

	// シェーダータイプに応じてコンパイル引数を設定
    std::wstring profile;
	
    switch (type)
    {
		// 頂点シェーダー
    case SETSHADERTYPE::VERTEXSHADER:
		profile = L"vs_6_0";
        break;
		// ピクセルシェーダー
    case SETSHADERTYPE::PIXSELSHADER:
		profile = L"ps_6_0";
        break;
		// コンピュートシェーダー
    case SETSHADERTYPE::CONPUTESHADER:
		profile = L"cs_6_0";
        break;
		// それ以外はエラー
    default:
        ErrorLog::OutputToConsole(u8"シェーダータイプの判定に失敗しました。ファイル名を確認してください。");
        return false;
    }

    // コマンド文字列作成
    std::wstring cmd = dxcPath.wstring() + L" \"" + shaderPath.wstring() + L"\""
        L" -T " + profile +
        L" -E main"
        L" -Fo \"" + outputPath.wstring() + L"\""
        L" -fspv-target-env=vulkan1.3";

    // 実行
    int ret = _wsystem(cmd.c_str());
    if (ret != 0)
    {
        ErrorLog::OutputToConsole(u8"SPIR-V 生成に失敗しました: " + shaderPath.u8string());
        return false;
    }

    return true;
}
