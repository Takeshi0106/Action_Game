
// ==================================
// ヘッダー
// ==================================
// 必須ヘッダー
#include "ShaderUtility.h"
// レポートメッセージ
#include "ReportMessage.h"


// ==================================
// ファイル名からシェーダータイプを取得する関数
// ==================================
SETSHADERTYPE ShaderUtility::GetShaderTypeFromFileName(const String& fileName)
{
	// ファイル名に "vs" が含まれている場合は頂点シェーダー
	if (fileName.GetU8String().find(u8"VS_") != std::u8string::npos) {
		return SETSHADERTYPE::VERTEXSHADER;
	}
	// ファイル名に "ps" が含まれている場合はピクセルシェーダー
	else if (fileName.GetU8String().find(u8"PS_") != std::u8string::npos) {
		return SETSHADERTYPE::PIXSELSHADER;
	}
	// ファイル名に "cs" が含まれている場合はコンピュートシェーダー
	else if (fileName.GetU8String().find(u8"CS_") != std::u8string::npos) {
		return SETSHADERTYPE::CONPUTESHADER;
	}
	else {
		ErrorLog::OutputToConsole(u8"ファイル名からシェーダータイプを判定できませんでした。確認してください");
		return SETSHADERTYPE::NONETYPE;
	}
}
