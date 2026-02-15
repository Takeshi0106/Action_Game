
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
	const String kShaderName = u8"ShaderName"; // シェーダーの名前

    const String kInputStart       = u8"InputLayout";    //入力レイアウト開始文字
    const String kCBufferStart     = u8"ConstantBuffer"; //定数バッファ開始文字
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
String ShaderInfo::Serialize(int spaceNumber) const
{
    String saveData; // データを入れる

    // セーブする情報を作成
    saveData += SaveUtils::MakeTypeInfo(kShaderName, m_ShaderName, spaceNumber);

    // 入力レイアウトの情報
    String ILData;
    for (int i = 0; i < (int)m_ILInfo.size(); i++)
    {
        ILData += SaveUtils::FormatAnonymousBlock(m_ILInfo[i].Serialize(spaceNumber + 2), spaceNumber + 1);
    }
    saveData += SaveUtils::FormatBlock(kInputStart, (int)m_ILInfo.size(), ILData, spaceNumber);

    // 定数バッファの情報
    String CBData;
    for (int i = 0; i < (int)m_CBInfo.size(); i++)
    {
        CBData += SaveUtils::FormatAnonymousBlock(m_CBInfo[i].Serialize(spaceNumber + 2), spaceNumber + 1);
    }
    saveData += SaveUtils::FormatBlock(kCBufferStart, (int)m_CBInfo.size(), CBData, spaceNumber);

    return saveData;
}


// ==================================
// ロード
// ==================================
bool ShaderInfo::Deserialize(const StringView& data)
{
    // ブロックを入れる配列
    StringView shaderNameStr;
    StringView inputLayoutBlock;
    StringView constantBufferBlock;

    // シェーダー名取り出し
    LoadUtils::ExtractTypeInfo(data, kShaderName, shaderNameStr);

    // 入力インプットブロックを取り出し
    if (!LoadUtils::ExtractBlocks(data, kInputStart, inputLayoutBlock)) {
        ErrorLog::OutputToConsole(u8"入力レイアウトブロックを取り出せませんでした");
        return false;
    }

    // 定数バッファブロックを取り出し
    if (!LoadUtils::ExtractBlocks(data, kCBufferStart, constantBufferBlock)) {
        ErrorLog::OutputToConsole(u8"定数バッファブロックを取り出せませんでした");
        return false;
    }

    // 名前を代入
    m_ShaderName = shaderNameStr;

    // 入力レイアウト情報を読み込む
    std::vector<StringView> ILStringData;
    if (!LoadUtils::ExtractSubBlocks(inputLayoutBlock, kInputStart, ILStringData)) {
        ErrorLog::OutputToConsole(u8"ShaderInfo : 入力ブロックデータを読み込むことが出来ませんでした。");
        return false;
    }
    m_ILInfo.resize(ILStringData.size()); // サイズを決める

    for (size_t i = 0; i < ILStringData.size(); i++)
    {
        if (!m_ILInfo[i].Deserialize(ILStringData[i])) {
            ErrorLog::OutputToConsole(u8"入力レイアウト : 文字列を読み込むことが出来ませんでした");
            return false;
        }
    }

    // 定数バッファ情報を読み込む
    std::vector<StringView> CBStringData;
    if (!LoadUtils::ExtractSubBlocks(constantBufferBlock, kCBufferStart, CBStringData)) {
        ErrorLog::OutputToConsole(u8"ShaderInfo : 定数バッファブロックデータを読み込むことが出来ませんでした。");
        return false;
    }
    m_CBInfo.resize(CBStringData.size()); // サイズを決める

    for (size_t i = 0; i < CBStringData.size(); i++)
    {
        if (!m_CBInfo[i].Deserialize(CBStringData[i])) {
            ErrorLog::OutputToConsole(u8"定数バッファ : 文字列を読み込むことが出来ませんでした");
            return false;
        }
    }

    return true;
}


