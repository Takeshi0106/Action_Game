#pragma once

// =================================================
// 【ヘッダーの概要】
// 定数バッファに必要なデータのクラス
// =================================================


// =======================================
// ヘッダー
// =======================================
#include <string> // 文字列を扱うためのヘッダー
#include "BaseSaveLoadObject.h" // セーブロードを行うオブジェクトの基底クラス


// ===========================================
// クラス
// ===========================================
class ConstantBufferInfo : public BaseSaveLoadObject
{
private:
	std::string m_Name = "";  // 定数バッファの名前
	int m_RegisterNumber = 0; // レジスタ番号
	size_t m_Size = 0;        // 大きさ１６の倍数にする

public:
	// コンストラクタ・デストラクタ
	ConstantBufferInfo() = default;
	~ConstantBufferInfo() override = default;

	// セーブ・ロード
	std::string Serialize(int space)const override;
	bool Deserialize(const std::string& data) override;

	// セッター
	void SetName(const std::string& name) { m_Name = name; }
	void SetRegisterNumber(int number) { m_RegisterNumber = number; }
	void SetSize(size_t size) { m_Size = size; }

	// ゲッター
	const std::string& GetName() const { return m_Name; }
	int GetRegisterNumber() const { return m_RegisterNumber; }
	size_t GetSize() const { return m_Size; }
};

