#pragma once

// =============================
// 【クラス概要】
// シェーダーの情報を代入するクラス
// =============================


// =============================
// ヘッダー
// =============================
// 基底クラスヘッダー
#include "BaseSaveLoadObject.h" // セーブロードヘッダー
// 定数バッファクラス
#include "ConstantBufferInfo.h"
// 入力レイアウトクラス
#include "InputLayoutInfo.h"
// 文字列
#include <string>      // 文字列
#include <string_view> // 文字列参照
// 配列
#include <vector>


// ==============================
// クラス
// ===============================
class ShaderInfo : public BaseSaveLoadObject
{
private:
	std::string m_ShaderName = "";            // シェーダーの名前
	std::vector<ConstantBufferInfo> m_CBInfo; // 定数バッファ情報
	std::vector<InputLayoutInfo> m_ILInfo;    // 入力レイアウトの情報


public:
	// コンストラクタ・デストラクタ
	ShaderInfo() = default;
	~ShaderInfo() override = default;

	// セーブ・ロード
	std::string Serialize(int space)const override;
	bool Deserialize(const std::string_view& data) override;

	// セッター
	void SetShaderName(const std::string& name) { m_ShaderName = name; }
	void SetConstantBufferInfo(std::vector<ConstantBufferInfo>& cB) { m_CBInfo = std::move(cB); }
	void SetInputLayoutInfo(std::vector<InputLayoutInfo>& iL) { m_ILInfo = std::move(iL); }

	// ゲッター
	const std::string& GetShaderName() const { return m_ShaderName; }
	const std::vector<ConstantBufferInfo>& GetConstantBufferInfo() const { return m_CBInfo; }
	const std::vector<InputLayoutInfo>& GetInputLayoutInfo() const { return m_ILInfo; }
};

