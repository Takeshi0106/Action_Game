
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


// ==========================================
// プロトタイプ宣言
// ==========================================
namespace {
    // 最初の区切り位置を渡して、それに対応する終わり区切り位置を代入する
    bool FindBlockEnd(const std::string_view& data, const size_t& startPos, size_t& endPos);
}


// =========================================
// セーブ関数
// =========================================
namespace SaveUtils {
	// データの種類とデータ文字列を受け取り、キーを付けて文字列にして返す
	std::string MakeTypeInfo(const std::string& dataType, const std::string& data, int spaceNumber)
	{
		std::string saveData(spaceNumber, ' '); // 空白を開ける

        saveData += dataType + kKey + data + "\n"; // セーブ文字列を作成

		return saveData;
	}


    // ブロックごとにする
    std::string FormatAnonymousBlock(const std::string& data, int spaceNumber)
    {
        std::string block;
        std::string space(spaceNumber, ' ');
        block += space + kBlockStart + data + space + kBlockEnd;
        return block;
    }


    // ブロックごとに作成
    std::string FormatBlock(const std::string& blockName, const int blockNumber, const std::string& data, int spaceNumber)
    {
        std::string block;
        std::string space(spaceNumber, ' ');

        block += space + blockName + " " + kBlockStart;      // ブロック開始
        block += space + kBlockNumber + std::to_string(blockNumber) + "\n\n"; // 要素数
        block += data;                                      // ブロック内データ
        block += space + kBlockEnd;                         // ブロック終了

        return block;
    }


}


// =========================================
// ロード関数
// =========================================
namespace LoadUtils {
    // データからデータの種類の内容を返す関数
    bool ExtractTypeInfo(const std::string_view& data, const std::string& type, std::string_view& info)
    {
        size_t infoLineStartPos = data.find(type); // データの種類がある行を探す
        if (infoLineStartPos == std::string::npos) {
            ErrorLog::OutputToConsole("データの種類が見つかりませんでした");
            return false;
        }
        size_t infotypeEndPos = data.find(kKey, infoLineStartPos);
        if (infotypeEndPos == std::string_view::npos) {
            ErrorLog::OutputToConsole("区切り文字が見つかりませんでした");
            return false;
        }

        size_t infoLineEndPos = data.find("\n", infoLineStartPos);
        if (infoLineEndPos == std::string_view::npos) {
            infoLineEndPos = data.size(); // 改行がなければ末尾まで
        }


        info = std::string_view(data.data() + infotypeEndPos + kKey.size(),
            infoLineEndPos - (infotypeEndPos + kKey.size()));

        return true;
    }


	// 文字列を解析して、データの種類をキーにしてデータを配列に代入する
	std::unordered_map<std::string_view, std::string_view> AllExtractTypeInfo(const std::string_view& data)
	{
        std::unordered_map<std::string_view, std::string_view> dataInfo; // データを入れる配列
        size_t pos = 0; // 今の位置

        while (pos < data.size())
        {
            size_t nextPos = data.find('\n', pos); // １行の終わりの位置
            std::string_view line; // 一行を入れる

            if (nextPos == std::string::npos)
            {
                line = std::string_view(data.data() + pos, data.size() - pos); // 今の位置から最後までを取り出す
                pos = data.size();       // 位置をデータの最後にする
            }
            else
            {
                line = std::string_view(data.data() + pos, nextPos - pos); // １行を取り出す
                pos = nextPos + 1; // 次の位置を求める
            }

            // 先頭の空白を削除 
            size_t firstNonSpace = line.find_first_not_of(' ');
            if (firstNonSpace != std::string_view::npos)
            {
                line.remove_prefix(firstNonSpace);
            }
            else
            {
                continue; // 空行はスキップ
            }

            // キーと値を分割
            size_t delimPos = line.find(kKey); // 区切り文字が出てくる位置を検索

            if (delimPos != std::string::npos)
            {
                std::string_view dataType = line.substr(0, delimPos); // キーまでを代入
                std::string_view dataString = line.substr(delimPos + kKey.size()); // キーから上を代入
                dataInfo[dataType] = dataString; // 配列に代入
            }
        }

        return dataInfo;
	}


    // データからブロックの情報を取り出す
    bool ExtractBlocks(const std::string_view& data, const std::string& blockName, std::string_view& block)
    {
        // ブロック名の位置を探す
        size_t startNamePos = data.find(blockName, 0);
        if (startNamePos == std::string::npos) {
            ErrorLog::OutputToConsole("ブロック名が見つかりませんでした");
            return false;
        }

        // ブロック情報の開始位置を探す
        size_t blockStartPos = data.find(kBlockStart, startNamePos + blockName.size()); // ブロック数の位置を探す
        if (blockStartPos == std::string::npos) {
            ErrorLog::OutputToConsole("ブロック情報の開示位置が見つかりませんでした");
            return false;
        }

        // 対応する終了位置を探す
        size_t blockEndPos;
        if (!FindBlockEnd(data, blockStartPos, blockEndPos)) {
            ErrorLog::OutputToConsole("対応する区切り文字が見つかりませんでした");
            return false;
        }

        // 最初から最後までの範囲を std::string_view に代入
        block = std::string_view(data.data() + startNamePos,
            blockEndPos - startNamePos + kBlockEnd.size());

        return true;
    }


    // ブロックの中身を取得する（指定した blockName の直下ブロックをすべて返す）
    bool ExtractSubBlocks(const std::string_view& data, const std::string& blockName, std::vector<std::string_view>& blocks)
    {
        // ブロック名の位置を探す
        size_t startNamePos = data.find(blockName, 0);
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

        std::string numberStr = std::string(data.data() + numberStart, numberEnd - numberStart); // 開始と終わりまでの文字列を代入
        int blockNumber = std::stoi(numberStr); // ブロック数を取得

        blocks.resize(blockNumber); // 配列をリサイズする

        // ブロック情報がなければ戻る
        if (blockNumber == 0) {
            return true;
        }

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

            // 対応する区切り文字を探す
            if (!FindBlockEnd(data, blockInfoStartPos, blockInfoEndPos)) {
                ErrorLog::OutputToConsole("対応する区切り文字が見つかりませんでした");
                return false;
            }

            // 中身だけを抽出して配列に追加
            std::string_view blockContent(data.data() + blockInfoStartPos + kBlockStart.size(),
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


// ==================================
// 関数
// ==================================
namespace {
        // 最初の区切り文字と対応する、区切り終わり文字の位置を返す
        bool FindBlockEnd(const std::string_view& data, const size_t& startPos, size_t& endPos)
        {
            size_t nextStartPos = data.find(kBlockStart, startPos + kBlockStart.size()); // 次の区切り開始位置
            size_t nextEndPos = data.find(kBlockEnd, startPos + kBlockStart.size()); // 次の区切り終了位置

            if (nextEndPos == std::string_view::npos) 
            {
                ErrorLog::OutputToConsole("データに区切り終了文字が含まれていません");
                return false; // 終了文字なし
            }
            int depth = 0; // 深度

            // 対応する区切り文字探し
            while (true)
            {
                if (nextStartPos != std::string_view::npos && nextStartPos < nextEndPos)
                {
                    depth++;
                    nextStartPos = data.find(kBlockStart, nextStartPos + kBlockStart.size());
                }
                else
                {
                    if (depth == 0)
                    {
                        endPos = nextEndPos; // 区切り終了位置を代入
                        return true;
                    }

                    depth--; // 深度を減らす
                    nextEndPos = data.find(kBlockEnd, nextEndPos + kBlockEnd.size()); // 終わりを探す
                    if (nextEndPos == std::string_view::npos) {
                        ErrorLog::OutputToConsole("対応する区切り文字が見つかりませんでした");
                        return false;
                    }
                }
            }

            return false;
        }
}

