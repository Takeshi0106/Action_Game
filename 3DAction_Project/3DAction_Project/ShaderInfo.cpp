
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


// =========================================
// グローバル変数
// =========================================
namespace {
	const std::string kShaderName = "ShaderName :"; // シェーダーの名前

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
    saveData += SaveUtils::MakeSaveData(kShaderName, m_ShaderName, spaceNumber);

    // 入力レイアウトの情報
    std::string ILData;
    for (int i = 0; i < (int)m_ILInfo.size(); i++)
    {
        ILData += SaveUtils::MakeNotNameBlock(m_ILInfo[i].Serialize(spaceNumber + 2), spaceNumber + 2); // 内容
    }
    saveData += SaveUtils::MakeBlock(kInputStart, (int)m_ILInfo.size(), ILData, spaceNumber + 1);

    // 定数バッファの情報
    std::string CBData;
    for (int i = 0; i < (int)m_CBInfo.size(); i++)
    {
        CBData += SaveUtils::MakeNotNameBlock(m_CBInfo[i].Serialize(spaceNumber + 2), spaceNumber + 2); // 内容
    }
    saveData += SaveUtils::MakeBlock(kCBufferStart, (int)m_CBInfo.size(), CBData, spaceNumber + 1);

    return saveData;
}


// ==================================
// ロード
// ==================================
bool ShaderInfo::Deserialize(const std::string& data)
{
    std::string shaderNameStr;
    std::string inputLayoutBlock;
    std::string constantBufferBlock;

    BlockType type = BlockType::NOT;

    size_t pos = 0;
    size_t dataSize = data.size();

    while (pos < dataSize)
    {
        size_t nextPos = data.find('\n', pos);
        std::string line = (nextPos == std::string::npos) ? data.substr(pos) : data.substr(pos, nextPos - pos);

        // 先頭空白削除
        while (!line.empty() && line.front() == ' ')
        {
            line.erase(line.begin());
        }

        // シェーダー名
        if (line.rfind(kShaderName, 0) == 0)
        {
            shaderNameStr = line.substr(kShaderName.size());
            type = BlockType::NOT;
        }
        // 入力レイアウト
        else if (line.rfind("InputLayout", 0) == 0)
        {
            inputLayoutBlock += line + "\n";
            type = BlockType::ILAYOUT;
        }
        // 定数バッファ
        else if (line.rfind("ConstantBuffer", 0) == 0)
        {
            constantBufferBlock += line + "\n";
            type = BlockType::CBUFFER;
        }
        else
        {
            // 現在のタイプに代入
            switch (type)
            {
                // 入力レイアウト
            case BlockType::ILAYOUT:
                inputLayoutBlock += line + "\n";
                break;

                // 定数バッファ
            case BlockType::CBUFFER:
                constantBufferBlock += line + "\n";
                break;

            default:
                break;

            }
        }

        pos = (nextPos == std::string::npos) ? dataSize : nextPos + 1;
    }

    return true;
}


