
// =========================================
// ヘッダー
// =========================================
#include "SaveLoadUtils.h" // 必須ヘッダー
#include "ReportMessage.h" // エラーログ


// =========================================
// グローバル変数
// =========================================
namespace {
	// データの種類とデータを区切る文字
	const std::string kKey = " :";

    // ブロック区切り
    const std::string kBlockStart = "{ \n";
    const std::string kBlockEnd = "} \n";

    // ブロックの数
    const std::string kBlockNumber = "BlockNumber :";
}


// =========================================
// セーブ関数
// =========================================
namespace SaveUtils {
	// データの種類とデータ文字列を受け取り、キーを付けて文字列にして返す
	std::string MakeSaveData(const std::string& dataType, const std::string& data, int spaceNumber)
	{
		std::string saveData(spaceNumber, ' '); // 空白を開ける

        saveData += dataType + kKey + data + "\n"; // セーブ文字列を作成

		return saveData;
	}


    // ブロックごとにする
    std::string MakeNotNameBlock(const std::string& data, int spaceNumber) 
    {
        std::string block;
        std::string space(spaceNumber, ' ');
        block += space + kBlockStart + data + space + kBlockEnd;
        return block;
    }


    // ブロックごとに作成
    std::string MakeBlock(const std::string& blockName, const int blockNumber, const std::string& data, int spaceNumber)
    {
        std::string block;
        std::string space(spaceNumber, ' ');

        block += space + blockName + " " + kBlockStart;      // ブロック開始
        block += space + kBlockNumber + std::to_string(blockNumber) + "\n"; // 要素数
        block += data;                                      // ブロック内データ
        block += space + kBlockEnd;                         // ブロック終了

        return block;
    }


}


// =========================================
// ロード関数
// =========================================
namespace LoadUtils {
	// 文字列を解析して、データの種類をキーにしてデータを配列に代入する
	std::unordered_map<std::string, std::string> ParseStringData(const std::string& data)
	{
        std::unordered_map<std::string, std::string> dataInfo; // データを入れる配列
        size_t pos = 0; // 今の位置

        while (pos < data.size())
        {
            size_t nextPos = data.find('\n', pos); // １行の終わりの位置
            std::string line; // 一行を入れる

            if (nextPos == std::string::npos)
            {
                line = data.substr(pos); // 今の位置から最後までを取り出す
                pos = data.size();       // 位置をデータの最後にする
            }
            else
            {
                line = data.substr(pos, nextPos - pos); // １行を取り出す
                pos = nextPos + 1; // 次の位置を求める
            }

            // 先頭の空白を削除
            while (!line.empty() && line.front() == ' ')
            {
                line.erase(line.begin());
            }

            // キーと値を分割
            size_t delimPos = line.find(kKey); // 区切り文字が出てくる位置を検索

            if (delimPos != std::string::npos)
            {
                std::string dataType = line.substr(0, delimPos); // キーまでを代入
                std::string dataString = line.substr(delimPos + kKey.size()); // キーから上を代入
                dataInfo[dataType] = dataString; // 配列に代入
            }
        }

        return dataInfo;
	}


    // ブロックの中身を取得する（指定した blockName の直下ブロックをすべて返す）
    bool ExtractBlocks(const std::string& data, const std::string& blockName, std::vector<std::string>& blocks)
    {
        size_t pos = 0; // 今の位置
        size_t dataSize = data.size(); // データの大きさ
        int blockNumber = 0; // ブロック数を入れる

        // ブロック名の位置を探す
        size_t startNamePos = data.find(blockName, pos);
        if (startNamePos == std::string::npos) {
            ErrorLog::OutputToConsole("ブロック名が見つかりませんでした");
            return false;
        }

        // ブロック情報の数を取得する
        size_t BlockNumberPos = data.find(kBlockNumber, startNamePos); // ブロック数の位置を探す
        if (BlockNumberPos == std::string::npos) {
            ErrorLog::OutputToConsole("ブロック情報の数が見つかりませんでした");
            return false;
        }

        size_t numberStart = BlockNumberPos + kBlockNumber.size(); // ブロック数の開始位置を求める
        size_t numberEnd = data.find_first_of("\r\n ", numberStart); // ブロック数の最後位置を求める
        if (numberEnd == std::string::npos) {
            ErrorLog::OutputToConsole("ブロック数の後に改行、空白などがありません");
            return false;
        }

        std::string numberStr = data.substr(numberStart, numberEnd - numberStart); // 開始と終わりまでの文字列を代入
        blockNumber = std::stoi(numberStr); // ブロック数を取得

        blocks.resize(blockNumber); // 配列をリサイズする

        // ブロック情報の開始位置を探す
        size_t blockInfoStartPos = data.find(kBlockStart, numberEnd); // ブロック数の位置を探す
        if (blockInfoStartPos == std::string::npos) {
            ErrorLog::OutputToConsole("ブロック情報の開示位置が見つかりませんでした");
            return false;
        }

        // ブロック情報を抜き出して、配列に代入させる
        for (int i = 0; i < blockNumber; i++)
        {
            size_t blockInfoEndPos = 0; // ブロック情報の終わり位置を入れる

            size_t nextStartPos = data.find(kBlockStart, blockInfoStartPos + kBlockStart.size()); // 区切り開始文字を探す
            size_t nextEndPos = data.find(kBlockEnd, blockInfoStartPos + kBlockStart.size());     // 区切り終わり文字を探す

            int blockDepth = 0; // 区切り文字の深さ、０になるとループを抜ける

            if (nextEndPos == std::string::npos) {
                ErrorLog::OutputToConsole("区切り終わり位置がありませんでした");
                return false;
            }

            // ブロック情報の開始に対応する終わり区切り文字の位置を探す
            while (true)
            {
                // 終わり文字より前に開始文字がある場合
                if (nextStartPos != std::string::npos && nextStartPos < nextEndPos)
                {
                    blockDepth++;
                    nextStartPos = data.find(kBlockStart, nextStartPos + kBlockStart.size()); // 次の開始位置を探す
                }
                else
                {
                    if (blockDepth == 0)
                    {
                        blockInfoEndPos = nextEndPos;
                        break;
                    }

                    blockDepth--;
                    nextEndPos = data.find(kBlockEnd, nextEndPos + kBlockEnd.size()); // 次の終わり位置を探す
                }

                if (nextEndPos == std::string::npos) {
                    ErrorLog::OutputToConsole("終わり区切り文字が見つかりませんでした");
                    return false;
                }
            }

            // 中身だけを抽出して配列に追加
            std::string blockContent = data.substr(blockInfoStartPos + kBlockStart.size(),
                blockInfoEndPos - blockInfoStartPos - kBlockStart.size());
            blocks[i] = blockContent;

            // 次のブロック開始位置に移動
            blockInfoStartPos = data.find(kBlockStart, blockInfoEndPos + kBlockEnd.size());
            if (i + 1 < blockNumber) {
                if (blockInfoStartPos == std::string::npos) {
                    ErrorLog::OutputToConsole("次のブロック開始位置を見つけられませんでした");
                }
            }
        }

        return true;
    }



}

