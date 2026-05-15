
// ==========================================
// ヘッダー
// ==========================================
// 必須ヘッダー
#include "SPIRVReflectionUtils.h"
// リファレンスヘッダー
extern "C" {
#include "spirv_reflect.h"
}
// レポートログ出力
#include "../../../ReportMessage.h"
// 配列ヘッダー
#include <vector>


// ==========================================
// SPIR-Vフォーマットを自作フォーマットに変換する関数
// ==========================================
VertexFormat ConvertSpvFormatToVertexFormat(SpvReflectFormat spvFormat);


// ==========================================
// SPIR-Vリフレクション構造体を自作のリファレクション構造体に変換する関数
// ==========================================
bool SPIRV::ReflectionUtils::ConvertSpvReflectToSelfRefData(
	const SpvReflectShaderModule& _spvModule,
	SelfReflectionInfo& _selfRefInfo)
{
	// リサイズ
	_selfRefInfo.ilInfos.reserve(_spvModule.input_variable_count);
	_selfRefInfo.cbInfos.reserve(_spvModule.descriptor_binding_count);

	// 入力レイアウト
	for (uint32_t i = 0; i < _spvModule.input_variable_count; i++)
	{
		// 入力変数の情報を取得
		SpvReflectInterfaceVariable* inputVar = _spvModule.input_variables[i];

		// データを自作のリファレクション構造体に変換して保存
		Self_ILInfo ilInfo;

		ilInfo.name = inputVar->name ? String(reinterpret_cast<const char8_t*>(inputVar->name)) : String(u8"Unnamed");
		ilInfo.index = inputVar->location;
		ilInfo.format = ConvertSpvFormatToVertexFormat(inputVar->format);

		if (ilInfo.format == VertexFormat::None)
		{
			ErrorLog::OutputToConsole(u8"入力レイアウトのフォーマットの変換に失敗しました。対応するフォーマットがありません。");
			return false;
		}

		_selfRefInfo.ilInfos.push_back(ilInfo);
	}


	// -------------------------------------------------
	// 定数バッファ
	// -------------------------------------------------
	for (uint32_t i = 0; i < _spvModule.descriptor_binding_count; i++)
	{
		// ディスクリプタバインディングの情報を取得
		SpvReflectDescriptorBinding* descriptorBinding = &_spvModule.descriptor_bindings[i];

		if (descriptorBinding->descriptor_type == SPV_REFLECT_DESCRIPTOR_TYPE_UNIFORM_BUFFER)
		{

			// データを自作のリファレクション構造体に変換して保存
			Self_CBInfo cbInfo;

			cbInfo.name = descriptorBinding->name ? String(reinterpret_cast<const char8_t*>(descriptorBinding->name)) : String(u8"Unnamed");
			cbInfo.index = static_cast<uint16_t>(descriptorBinding->binding);
			cbInfo.size = descriptorBinding->block.size;

			_selfRefInfo.cbInfos.push_back(cbInfo);
		}
	}

	return true;
}


// -----------------------------------------
// SPIR-Vフォーマットを自作フォーマットに変換する関数
// -----------------------------------------
VertexFormat ConvertSpvFormatToVertexFormat(SpvReflectFormat spvFormat)
{
	switch (spvFormat)
	{
	case SPV_REFLECT_FORMAT_R32G32_SFLOAT:
		return VertexFormat::Float2;
		break;

	case SPV_REFLECT_FORMAT_R32G32B32_SFLOAT:
		return VertexFormat::Float3;
		break;

	case SPV_REFLECT_FORMAT_R32G32B32A32_SFLOAT:
		return VertexFormat::Float4;
		break;

	case SPV_REFLECT_FORMAT_R32G32B32A32_UINT:
		return VertexFormat::Uint4;
		break;

	default:
		ErrorLog::OutputToConsole(u8"SPIR-Vフォーマットの変換に失敗しました。対応するフォーマットがありません。");
		return VertexFormat::None;
	}
}
