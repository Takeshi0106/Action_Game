#pragma once

// =================================================
// 【ヘッダーの概要】
// シェーダー関連で必要なデータのクラス
// =================================================


// =======================================
// ヘッダー
// =======================================
// 文字列を扱うためのヘッダー
#include "UTF8_String.h"
#include "UTF8_StringView.h"
// セーブロードを行うオブジェクトの基底クラス
#include "BaseSaveLoadObject.h"
// 固定整数型
#include <cstdint>


// =========================================
// クラス
// =========================================
class InputLayoutInfo : public BaseSaveLoadObject
{
private:
	// セマンティックの名前
	String m_SemanticName = u8"";
	// セマンティックの番号
	uint16_t m_SemanticIndex = 0;
	// スロット番号
	uint16_t m_InputSlot = 0;
	// データの形式 (DXGI)
	uint16_t m_Format = 0;

public:
	// コンストラクタ・デストラクタ
	InputLayoutInfo() = default;
	~InputLayoutInfo() override = default;

	// セーブ・ロード
	String Serialize(int space)const override;
	bool Deserialize(const StringView& data) override;

	// セッター
	void SetSemanticName(const String& name) { m_SemanticName = name; }
	void SetSemanticIndex(uint16_t index) { m_SemanticIndex = index; }
	void SetInputSlot(uint16_t slot) { m_InputSlot = slot; }
	void SetFormat(uint16_t format) { m_Format = format; }

	// ゲッター
	const String& GetSemanticName() const { return m_SemanticName; }
	uint16_t GetSemanticIndex() const { return m_SemanticIndex; }
	uint16_t GetInputSlot() const { return m_InputSlot; }
	uint16_t GetFormat() const { return m_Format; }
};

