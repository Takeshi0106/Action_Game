#pragma once

// =================================================
// 【ヘッダーの概要】
// 定数バッファに必要なデータのクラス
// =================================================


// =======================================
// ヘッダー
// =======================================
#include <string> // 文字列を扱うためのヘッダー
#include <string_view> // 文字列参照
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
	std::string m_Name = "";
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
	std::string Serialize(int _space)const override final;
	bool Deserialize(const std::string_view& _data) override final;

	// ---------------------------------
	// セッター
	// ---------------------------------
	void SetName(const std::string& _name) { m_Name = _name; }
	void SetRegisterNumber(uint16_t _number) { m_RegisterNumber = _number; }
	void SetSize(size_t _size) { m_Size = _size; }

	// ---------------------------------
	// ゲッター
	// ---------------------------------
	const std::string& GetName() const { return m_Name; }
	const uint16_t GetRegisterNumber() const { return m_RegisterNumber; }
	const size_t GetSize() const { return m_Size; }
};

