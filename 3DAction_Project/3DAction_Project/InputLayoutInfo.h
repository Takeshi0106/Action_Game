#pragma once

// =================================================
// 【ヘッダーの概要】
// シェーダー関連で必要なデータのクラス
// =================================================


// =======================================
// ヘッダー
// =======================================
#include <string> // 文字列を扱うためのヘッダー
#include "BaseSaveLoadObject.h" // セーブロードを行うオブジェクトの基底クラス


// =========================================
// クラス
// =========================================
class InputLayoutInfo : public BaseSaveLoadObject
{
private:
	std::string m_SemanticName = ""; // セマンティックの名前
	int m_SemanticIndex = 0;         // セマンティックの番号
	int m_InputSlot = 0;             // スロット番号
	int m_Format = 0;                // データの形式 (DXGI)

public:
	// コンストラクタ・デストラクタ
	InputLayoutInfo() = default;
	~InputLayoutInfo() override = default;

	// セーブ・ロード
	std::string Serialize(int space)const override;
	bool Deserialize(const std::string& data) override;

	// セッター
	void SetSemanticName(const std::string& name) { m_SemanticName = name; }
	void SetSemanticIndex(int index) { m_SemanticIndex = index; }
	void SetInputSlot(int slot) { m_InputSlot = slot; }
	void SetFormat(int format) { m_Format = format; }

	// ゲッター
	const std::string& GetSemanticName() const { return m_SemanticName; }
	int GetSemanticIndex() const { return m_SemanticIndex; }
	int GetInputSlot() const { return m_InputSlot; }
	int GetFormat() const { return m_Format; }
};

