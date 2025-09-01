
// =======================================
// ヘッダー
// =======================================
// 必須ヘッダー
#include "ShaderInfo.h"
// セーブロード時に使用する関数群
#include "SaveLoadUtils.h"
// キャスト失敗時にエラーを出すようヘッダー
#include <stdexcept>
// エラー出力関数
#include "ReportMessage.h"
// 配列
#include <vector>
#include <unordered_map>


// =========================================
// グローバル変数
// =========================================
namespace {
	const std::string kShaderName = "ShaderName"; // シェーダーの名前

    const std::string kInputStart       = "InputLayout";    //入力レイアウト開始文字
    const std::string kCBufferStart     = "ConstantBuffer"; //定数バッファ開始文字
}


// ========================================
// 列挙型
// ========================================
enum BlockType {
    ILAYOUT = 0,
    CBUFFER,
    NOT
};


// ==================================
// セーブ
// ==================================
std::string ShaderInfo::Serialize(int spaceNumber) const
{
    std::string saveData; // データを入れる

    // セーブする情報を作成
    saveData += SaveUtils::MakeTypeInfo(kShaderName, m_ShaderName, spaceNumber);

    // 入力レイアウトの情報
    std::string ILData;
    for (int i = 0; i < (int)m_ILInfo.size(); i++)
    {
        ILData += SaveUtils::FormatAnonymousBlock(m_ILInfo[i].Serialize(spaceNumber + 2), spaceNumber + 1); // 内容
    }
    saveData += SaveUtils::FormatBlock(kInputStart, (int)m_ILInfo.size(), ILData, spaceNumber);

    // 定数バッファの情報
    std::string CBData;
    for (int i = 0; i < (int)m_CBInfo.size(); i++)
    {
        CBData += SaveUtils::FormatAnonymousBlock(m_CBInfo[i].Serialize(spaceNumber + 2), spaceNumber + 1); // 内容
    }
    saveData += SaveUtils::FormatBlock(kCBufferStart, (int)m_CBInfo.size(), CBData, spaceNumber);

    return saveData;
}


// ==================================
// ロード
// ==================================
bool ShaderInfo::Deserialize(const std::string_view& data)
{
    // ブロックを入れる配列
    std::string_view shaderNameStr;
    std::string_view inputLayoutBlock;
    std::string_view constantBufferBlock;

    // シェーダー名取り出し
    LoadUtils::ExtractTypeInfo(data, kShaderName, shaderNameStr);

    // 入力インプットブロックを取り出し
    if (!LoadUtils::ExtractBlocks(data, kInputStart, inputLayoutBlock)) {
        ErrorLog::OutputToConsole("入力レイアウトブロックを取り出せませんでした");
        return false;
    }

    // 定数バッファブロックを取り出し
    if (!LoadUtils::ExtractBlocks(data, kCBufferStart, constantBufferBlock)) {
        ErrorLog::OutputToConsole("定数バッファブロックを取り出せませんでした");
        return false;
    }

    // 名前を代入
    m_ShaderName = shaderNameStr;

    // 入力レイアウト情報を読み込む
    std::vector<std::string_view> ILStringData;
    if (!LoadUtils::ExtractSubBlocks(inputLayoutBlock, kInputStart, ILStringData)) {
        ErrorLog::OutputToConsole("ShaderInfo : 入力ブロックデータを読み込むことが出来ませんでした。");
        return false;
    }
    m_ILInfo.resize(ILStringData.size()); // サイズを決める

    for (int i = 0; i < ILStringData.size(); i++)
    {
        if (!m_ILInfo[i].Deserialize(std::string(ILStringData[i]))) {
            ErrorLog::OutputToConsole("入力レイアウト : 文字列を読み込むことが出来ませんでした");
            return false;
        }
    }

    // 定数バッファ情報を読み込む
    std::vector<std::string_view> CBStringData;
    if (!LoadUtils::ExtractSubBlocks(constantBufferBlock, kCBufferStart, CBStringData)) {
        ErrorLog::OutputToConsole("ShaderInfo : 定数バッファブロックデータを読み込むことが出来ませんでした。");
        return false;
    }
    m_CBInfo.resize(CBStringData.size()); // サイズを決める

    for (int i = 0; i < CBStringData.size(); i++)
    {
        if (!m_CBInfo[i].Deserialize(std::string(CBStringData[i]))) {
            ErrorLog::OutputToConsole("定数バッファ : 文字列を読み込むことが出来ませんでした");
            return false;
        }
    }

    return true;
}


