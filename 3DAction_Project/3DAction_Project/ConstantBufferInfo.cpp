
// =======================================
// ヘッダー
// =======================================
// 必須ヘッダー
#include "ConstantBufferInfo.h"
// セーブロード時に使用する関数群
#include "SaveLoadUtils.h"
// キャスト失敗時にエラーを出すようヘッダー
#include <stdexcept>
// エラー出力関数
#include "ReportMessage.h"


// =========================================
// グローバル変数
// =========================================
namespace {
	// セーブ時に使用するデータタイプ文字列
	const String kCBufferName = u8"CBufferName";       // 定数バッファの名前
	const String kRegisterNumber = u8"RegisterNumber"; // レジスタ番号
	const String kSize = u8"Size";                     // バイト数 (16の倍数単位)
}


// ======================================
// セーブするデータを文字列にして返す関数
// ======================================
String ConstantBufferInfo::Serialize(int spaceNumber) const
{
    // データを入れる
    String saveData;

    // セーブする情報を作成
    saveData += SaveUtils::MakeTypeInfo(kCBufferName, m_Name, spaceNumber);
    saveData += SaveUtils::MakeTypeInfo(kRegisterNumber, String::to_u8string(m_RegisterNumber), spaceNumber);
    saveData += SaveUtils::MakeTypeInfo(kSize, String::to_u8string((uint64_t)m_Size), spaceNumber);

    return saveData;
}


// ======================================
// 文字列を受け取り、メンバー変数に代入する関数
// ======================================
bool ConstantBufferInfo::Deserialize(const StringView& data)
{
    // データを取得する
    std::unordered_map<Hashed_String, StringView> stringData = LoadUtils::AllExtractTypeInfo(data);

    // データをキャストして内容を取得する
    // 定数バッファ名
    auto it = stringData.find((Hashed_String)kCBufferName);
    if (it != stringData.end()) {
        m_Name = it->second;
    }
    else {
        ErrorLog::OutputToConsole(u8"定数バッファ：CBufferName が存在しません");
        return false;
    }

    // レジスタ番号
    it = stringData.find((Hashed_String)kRegisterNumber);
    if (it != stringData.end()) {
        // キャストに成功失敗したかの確認
        try {
			// uint16_t にキャスト (ANSIIのため同じになるはず)
            m_RegisterNumber = (uint16_t)std::stoi(reinterpret_cast<const char*>(String(it->second).GetU8Char()));
        }
        catch (const std::invalid_argument&) {
            ErrorLog::OutputToConsole(u8"定数バッファ：RegisterNumber のキャストに失敗しました");
            return false;
        }
    }
    else {
        ErrorLog::OutputToConsole(u8"定数バッファ：RegisterNumber が存在しません");
        return false;
    }

    // サイズ
    it = stringData.find((Hashed_String)kSize);
    if (it != stringData.end()) {
        // キャストに成功失敗したかの確認
        try {
			// size_t にキャスト (ANSIIのため同じになるはず)
            m_Size = static_cast<size_t>(std::stoul(reinterpret_cast<const char*>(String(it->second).GetU8Char())));
        }
        catch (const std::invalid_argument&) {
            ErrorLog::OutputToConsole(u8"定数バッファ：Size のキャストに失敗しました");
            return false;
        }
    }
    else {
        ErrorLog::OutputToConsole(u8"定数バッファ：Size が存在しません");
        return false;
    }


    return true;
}
