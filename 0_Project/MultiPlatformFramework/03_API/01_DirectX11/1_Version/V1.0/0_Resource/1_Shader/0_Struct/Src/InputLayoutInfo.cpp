
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
// 
#include <charconv>
// ハッシュ文字列ヘッダー
#include "Hashed_String.h"


// =========================================
// グローバル変数
// =========================================
namespace {
    // セーブ時に使用するデータタイプ文字列
    const Hashed_String kInputLayoutSemanticName{ u8"InputLayoutSemanticName" };  // セマンティック名
    const Hashed_String kInputLayoutSemanticIndex{ u8"InputLayoutSemanticIndex" }; // セマンティックインデックス
    const Hashed_String kInputLayoutInputSlot{ u8"InputLayoutInputSlot" };     // 入力スロット番号
    const Hashed_String kInputLayoutFormat{ u8"InputLayoutFormat" };        // フォーマット
}


// ======================================
// セーブするデータを文字列にして返す関数
// ======================================
String InputLayoutInfo::Serialize(int spaceNumber) const
{
    String saveData; // データを入れる

    // セーブする情報を作成
    saveData += SaveUtils::MakeTypeInfo(kInputLayoutSemanticName.GetString(), m_SemanticName, spaceNumber);
    saveData += SaveUtils::MakeTypeInfo(kInputLayoutSemanticIndex.GetString(), 
        String::to_u8string(static_cast<uint64_t>(m_SemanticIndex)), spaceNumber);
    saveData += SaveUtils::MakeTypeInfo(kInputLayoutInputSlot.GetString(), 
        String::to_u8string(static_cast<uint64_t>(m_InputSlot)), spaceNumber);
    saveData += SaveUtils::MakeTypeInfo(kInputLayoutFormat.GetString(), 
        String::to_u8string(static_cast<uint64_t>(m_Format)), spaceNumber);

    return saveData;
}


// ======================================
// 文字列を受け取り、メンバー変数に代入する関数
// ======================================
bool InputLayoutInfo::Deserialize(const StringView& data)
{
    // データを取得する
    std::unordered_map<Hashed_String, StringView> stringData = LoadUtils::AllExtractTypeInfo(data);

    // データをキャストして内容を取得する
    
    // セマンティック名
    auto it = stringData.find(kInputLayoutSemanticName);
    if (it != stringData.end()) 
    {
        m_SemanticName = it->second;
    }
    else 
    {
        ErrorLog::OutputToConsole(u8"入力レイアウト：SemanticName が存在しません");
        return false;
    }

    // セマンティックナンバー
    it = stringData.find(kInputLayoutSemanticIndex);
    if (it != stringData.end()) 
    {
        // 変換
        StringView str = it->second;
        auto [ptr, ec] = std::from_chars(
            reinterpret_cast<const char*>(str.GetData()),
            reinterpret_cast<const char*>(str.GetData() + str.GetSize()),
            m_SemanticIndex);

        if (ec == std::errc::invalid_argument) {
            ErrorLog::OutputToConsole(u8"数字ではない文字が含まれています");
        }
        else if (ec == std::errc::result_out_of_range) {
            ErrorLog::OutputToConsole(u8"値が型以上の範囲です。");
        }
    }
    else {
        ErrorLog::OutputToConsole(u8"入力レイアウト：SemanticIndex が存在しません");
        return false;
    }

    // 入力スロット
    it = stringData.find(kInputLayoutInputSlot);
    if (it != stringData.end()) {
        // 変換
        StringView str = it->second;
        auto [ptr, ec] = std::from_chars(
            reinterpret_cast<const char*>(str.GetData()), 
            reinterpret_cast<const char*>(str.GetData() + str.GetSize()), 
            m_InputSlot);

        if (ec == std::errc::invalid_argument) {
            ErrorLog::OutputToConsole(u8"数字ではない文字が含まれています");
        }
        else if (ec == std::errc::result_out_of_range) {
            ErrorLog::OutputToConsole(u8"値が型以上の範囲です。");
        }
    }
    else {
        ErrorLog::OutputToConsole(u8"入力レイアウト：InputSlot が存在しません");
        return false;
    }

    // フォーマット
    it = stringData.find(kInputLayoutFormat);
    if (it != stringData.end()) {
        // 変換
        StringView str = it->second;
        auto [ptr, ec] = std::from_chars(
            reinterpret_cast<const char*>(str.GetData()), 
            reinterpret_cast<const char*>(str.GetData() + str.GetSize()), 
            m_Format);

        if (ec == std::errc::invalid_argument) {
            ErrorLog::OutputToConsole(u8"数字ではない文字が含まれています");
        }
        else if (ec == std::errc::result_out_of_range) {
            ErrorLog::OutputToConsole(u8"値が型以上の範囲です。");
        }
    }
    else {
        ErrorLog::OutputToConsole(u8"入力レイアウト：Format が存在しません");
        return false;
    }

    return true;
}
