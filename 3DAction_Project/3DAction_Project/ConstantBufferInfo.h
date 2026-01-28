#pragma once

// =================================================
// 【ヘッダーの概要】
// 定数バッファに必要なデータのクラス
// =================================================


// =======================================
// ヘッダー
// =======================================
#include "UTF8_String.h"
#include "UTF8_StringView.h"
// 固定整数型
#include <cstddef>
#include "BaseSaveLoadObject.h" // セーブロードを行うオブジェクトの基底クラス


// ===========================================
// クラス
// ===========================================
class ConstantBufferInfo : public BaseSaveLoadObject
{
private:
	// ---------------------------------
	// メンバー変数
	// ---------------------------------
	// 定数バッファの名前
	String m_Name = u8"";
	// レジスタ番号
	uint16_t m_RegisterNumber = 0;
	// 必ず１６の倍数にする
	size_t m_Size = 0;

public:
	// ---------------------------------
	// コンストラクタ・デストラクタ
	// ---------------------------------
	ConstantBufferInfo() = default;
	~ConstantBufferInfo() override final = default;


	// ---------------------------------
	// セーブ・ロード
	// ---------------------------------
	String Serialize(int _space)const override final;
	bool Deserialize(const StringView& _data) override final;

	// ---------------------------------
	// セッター
	// ---------------------------------
	void SetName(const String& _name) { m_Name = _name; }
	void SetRegisterNumber(uint16_t _number) { m_RegisterNumber = _number; }
	void SetSize(size_t _size) { m_Size = _size; }

	// ---------------------------------
	// ゲッター
	// ---------------------------------
	const String& GetName() const { return m_Name; }
	const uint16_t GetRegisterNumber() const { return m_RegisterNumber; }
	const size_t GetSize() const { return m_Size; }
};

