
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
	const std::string kCBufferName = "CBufferName :";       // 定数バッファの名前
	const std::string kRegisterNumber = "RegisterNumber :"; // レジスタ番号
	const std::string kSize = "Size :";                     // バイト数 (16の倍数単位)
}


// ======================================
// セーブするデータを文字列にして返す関数
// ======================================
std::string ConstantBufferInfo::Serialize(int spaceNumber) const
{
    std::string saveData; // データを入れる

    // セーブする情報を作成
    saveData += SaveUtils::MakeSaveData(kCBufferName, m_Name, spaceNumber);
    saveData += SaveUtils::MakeSaveData(kRegisterNumber, std::to_string(m_RegisterNumber), spaceNumber);
    saveData += SaveUtils::MakeSaveData(kSize, std::to_string(m_Size), spaceNumber);

    return saveData;
}


// ======================================
// 文字列を受け取り、メンバー変数に代入する関数
// ======================================
bool ConstantBufferInfo::Deserialize(const std::string& data)
{
    // データを取得する
    std::unordered_map<std::string, std::string> stringData = LoadUtils::ParseStringData(data);

    // データをキャストして内容を取得する
    // 定数バッファ名
    auto it = stringData.find(kCBufferName);
    if (it != stringData.end()) {
        m_Name = it->second;
    }
    else {
        ErrorLog::OutputToConsole("定数バッファ：CBufferName が存在しません");
        return false;
    }

    // レジスタ番号
    it = stringData.find(kRegisterNumber);
    if (it != stringData.end()) {
        // キャストに成功失敗したかの確認
        try {
            m_RegisterNumber = std::stoi(it->second);
        }
        catch (const std::invalid_argument&) {
            ErrorLog::OutputToConsole("定数バッファ：RegisterNumber のキャストに失敗しました");
            return false;
        }
    }
    else {
        ErrorLog::OutputToConsole("定数バッファ：RegisterNumber が存在しません");
        return false;
    }

    // サイズ
    it = stringData.find(kSize);
    if (it != stringData.end()) {
        // キャストに成功失敗したかの確認
        try {
            m_Size = static_cast<size_t>(std::stoul(it->second));
        }
        catch (const std::invalid_argument&) {
            ErrorLog::OutputToConsole("定数バッファ：Size のキャストに失敗しました");
            return false;
        }
    }
    else {
        ErrorLog::OutputToConsole("定数バッファ：Size が存在しません");
        return false;
    }


    return true;
}