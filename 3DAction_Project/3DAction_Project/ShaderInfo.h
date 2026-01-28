#pragma once

// =============================
// 【クラス概要】
// シェーダーの情報を代入するクラス
// =============================


// =============================
// ヘッダー
// =============================
// 基底クラスヘッダー
#include "BaseSaveLoadObject.h" 
// 定数バッファクラス
#include "ConstantBufferInfo.h"
// 入力レイアウトクラス
#include "InputLayoutInfo.h"
// 文字列
#include "UTF8_String.h"
#include "UTF8_StringView.h"
// 配列
#include <vector>


// ==============================
// クラス
// ===============================
class ShaderInfo : public BaseSaveLoadObject
{
private:
	// シェーダーの名前
	String m_ShaderName = u8"";
	// 定数バッファ情報
	std::vector<ConstantBufferInfo> m_CBInfo;
	// 入力レイアウトの情報
	std::vector<InputLayoutInfo> m_ILInfo;


public:
	// ---------------------------------
	// コンストラクタ・デストラクタ
	// ---------------------------------
	ShaderInfo() = default;
	~ShaderInfo() override = default;

	// ---------------------------------
	// セーブ・ロード
	// ---------------------------------
	String Serialize(int space)const override;
	bool Deserialize(const StringView& data) override;

	// ---------------------------------
	// セッター
	// ---------------------------------
	void SetShaderName(const String& name) { m_ShaderName = name; }
	void SetConstantBufferInfo(std::vector<ConstantBufferInfo>& cB) { m_CBInfo = std::move(cB); }
	void SetInputLayoutInfo(std::vector<InputLayoutInfo>& iL) { m_ILInfo = std::move(iL); }

	// ---------------------------------
	// ゲッター
	// ---------------------------------
	const String& GetShaderName() const { return m_ShaderName; }
	const std::vector<ConstantBufferInfo>& GetConstantBufferInfo() const { return m_CBInfo; }
	const std::vector<InputLayoutInfo>& GetInputLayoutInfo() const { return m_ILInfo; }
};

