
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

	// 区切り文字を統一
	dxcPath = dxcPath.generic_string();

	// dxc.exe が存在するかチェック
    if (!std::filesystem::exists(dxcPath))
    {
        ErrorLog::OutputToConsole(u8"dxc.exe が見つかりません");
        return false;
    }

	// .hlslパス取得
    std::filesystem::path shaderPath = std::filesystem::absolute(
		std::filesystem::path(kHlslFolderPath.GetU8String()) /
        (_hlslName.GetU8String() + kHlslExtension.GetU8String()));

	// 区切り文字を統一
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

	// 出力先のディレクトリが存在しない場合は作成
    if (!std::filesystem::exists(outputPath.parent_path())) {
        if (!std::filesystem::create_directories(outputPath.parent_path())) {
            ErrorLog::OutputToConsole(u8"出力先のディレクトリの作成に失敗しました: " + outputPath.parent_path().u8string());
            return false;
        }
    }

    // コマンド文字列作成
    std::wstring cmd = dxcPath.wstring() + L" \"" + shaderPath.wstring() + L"\""
        L" -T " + profile +
        L" -E main"
        L" -spirv"
        L" -fvk-use-dx-layout"
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


// ==============================================
// SPIR-Vコンパイル外部ファイルを削除する
// ==============================================
bool Windows_SPIRV_CompileModule::DeleteCompileShader(const String& _hlslName)
{
    // 出力ファイルパス
    std::filesystem::path outputPath = std::filesystem::absolute(
        std::filesystem::path(kSPIRVFolderPath.GetU8String()) /
        (_hlslName.GetU8String() + kSPIRVExtension.GetU8String()));

    // 区切り文字を統一
    outputPath = outputPath.generic_string();

    // ファイルが存在する場合は削除
    if (std::filesystem::exists(outputPath))
    {
        // エラーコードを取得
        std::error_code ec;

        if (!std::filesystem::remove(outputPath, ec))
        {
            ErrorLog::OutputToConsole(u8"SPIR-Vファイルの削除に失敗しました: " + outputPath.u8string() + u8" エラー: " +
                String::to_u8string(ec.value()));
            return false;
        }
    }

    return true;
}
