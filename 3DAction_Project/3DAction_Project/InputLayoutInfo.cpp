
// =======================================
// ヘッダー
// =======================================
// 必須ヘッダー
#include "InputLayoutInfo.h"
// セーブロード時に使用する関数群
#include "SaveLoadUtils.h"
// キャスト失敗時にエラーを出すようヘッダー
#include <stdexcept>
// エラー出力関数
#include "ReportMessage.h"
// 文字列参照
#include <string_view>


// =========================================
// グローバル変数
// =========================================
namespace {
    // セーブ時に使用するデータタイプ文字列
    const std::string kInputLayoutSemanticName  = "InputLayoutSemanticName";  // セマンティック名
    const std::string kInputLayoutSemanticIndex = "InputLayoutSemanticIndex"; // セマンティックインデックス
    const std::string kInputLayoutInputSlot     = "InputLayoutInputSlot";     // 入力スロット番号
    const std::string kInputLayoutFormat        = "InputLayoutFormat";        // フォーマット
}


// ======================================
// セーブするデータを文字列にして返す関数
// ======================================
std::string InputLayoutInfo::Serialize(int spaceNumber) const
{
    std::string saveData; // データを入れる

    // セーブする情報を作成
    saveData += SaveUtils::MakeTypeInfo(kInputLayoutSemanticName , m_SemanticName                 , spaceNumber);
    saveData += SaveUtils::MakeTypeInfo(kInputLayoutSemanticIndex, std::to_string(m_SemanticIndex), spaceNumber);
    saveData += SaveUtils::MakeTypeInfo(kInputLayoutInputSlot    , std::to_string(m_InputSlot)    , spaceNumber);
    saveData += SaveUtils::MakeTypeInfo(kInputLayoutFormat       , std::to_string(m_Format)       , spaceNumber);

    return saveData;
}


// ======================================
// 文字列を受け取り、メンバー変数に代入する関数
// ======================================
bool InputLayoutInfo::Deserialize(const std::string_view& data)
{
    // データを取得する
    std::unordered_map<std::string_view, std::string_view> stringData = LoadUtils::AllExtractTypeInfo(data);

    // データをキャストして内容を取得する
    // セマンティック名
    auto it = stringData.find(kInputLayoutSemanticName);
    if (it != stringData.end()) {
        m_SemanticName = it->second;
    }
    else {
        ErrorLog::OutputToConsole("入力レイアウト：SemanticName が存在しません");
        return false;
    }

    // セマンティックナンバー
    it = stringData.find(kInputLayoutSemanticIndex);
    if (it != stringData.end()) {
        try {
            m_SemanticIndex = std::stoul(std::string(it->second));
        }
        catch (const std::invalid_argument&) {
            ErrorLog::OutputToConsole("入力レイアウト：SemanticIndex のキャストに失敗しました");
            return false;
        }
    }
    else {
        ErrorLog::OutputToConsole("入力レイアウト：SemanticIndex が存在しません");
        return false;
    }

    // 入力スロット
    it = stringData.find(kInputLayoutInputSlot);
    if (it != stringData.end()) {
        try {
            m_InputSlot = std::stoul(std::string(it->second));
        }
        catch (const std::invalid_argument&) {
            ErrorLog::OutputToConsole("入力レイアウト：InputSlot のキャストに失敗しました");
            return false;
        }
    }
    else {
        ErrorLog::OutputToConsole("入力レイアウト：InputSlot が存在しません");
        return false;
    }

    // フォーマット
    it = stringData.find(kInputLayoutFormat);
    if (it != stringData.end()) {
        try {
            m_Format = std::stoul(std::string(it->second));
        }
        catch (const std::invalid_argument&) {
            ErrorLog::OutputToConsole("入力レイアウト：Format のキャストに失敗しました");
            return false;
        }
    }
    else {
        ErrorLog::OutputToConsole("入力レイアウト：Format が存在しません");
        return false;
    }

    return true;
}