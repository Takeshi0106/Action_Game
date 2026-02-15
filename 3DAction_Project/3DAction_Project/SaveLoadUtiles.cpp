
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
	const String kKey = u8" :";

    // ブロック区切り
    const String kBlockStart = u8"{ \n";
    const String kBlockEnd = u8"} \n";

    // ブロックの数
    const String kBlockNumber = u8"BlockNumber :";
}


// ==========================================
// プロトタイプ宣言
// ==========================================
namespace {
    // 最初の区切り位置を渡して、それに対応する終わり区切り位置を代入する
    bool FindBlockEnd(const StringView& data, const size_t& startPos, size_t& endPos);
}


// =========================================
// セーブ関数
// =========================================
namespace SaveUtils {
	// データの種類とデータ文字列を受け取り、キーを付けて文字列にして返す
	String MakeTypeInfo(const String& dataType, const String& data, int spaceNumber)
	{
		String saveData(std::u8string(spaceNumber, ' ')); // 空白を開ける

        saveData += dataType + kKey + data + u8"\n"; // セーブ文字列を作成

		return saveData;
	}


    // ブロックごとにする
    String FormatAnonymousBlock(const String& data, int spaceNumber)
    {
        String block;
        String space(std::u8string(spaceNumber, ' '));
        block += space + kBlockStart + data + space + kBlockEnd;
        return block;
    }


    // ブロックごとに作成
    String FormatBlock(const String& blockName, const int blockNumber, const String& data, int spaceNumber)
    {
        String block;
        String space(std::u8string(spaceNumber, ' '));

        block += space + blockName + u8" " + kBlockStart;      // ブロック開始
        block += space + kBlockNumber + String::to_u8string((int64_t)blockNumber) + u8"\n\n"; // 要素数
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
    bool ExtractTypeInfo(const StringView& data, const String& type, StringView& info)
    {
        size_t infoLineStartPos = data.GetFind(type); // データの種類がある行を探す
        if (infoLineStartPos == std::string::npos) {
            ErrorLog::OutputToConsole(u8"データの種類が見つかりませんでした");
            return false;
        }
        size_t infotypeEndPos = data.GetFind(kKey, infoLineStartPos);
        if (infotypeEndPos == std::string_view::npos) {
            ErrorLog::OutputToConsole(u8"区切り文字が見つかりませんでした");
            return false;
        }

        size_t infoLineEndPos = data.GetFind(u8"\n", infoLineStartPos);
        if (infoLineEndPos == std::string_view::npos) {
            infoLineEndPos = data.GetSize(); // 改行がなければ末尾まで
        }


        info = StringView(data.GetData() + infotypeEndPos + kKey.GetSize(),
            infoLineEndPos - (infotypeEndPos + kKey.GetSize()));

        return true;
    }


	// 文字列を解析して、データの種類をキーにしてデータを配列に代入する
	std::unordered_map<Hashed_String, StringView> AllExtractTypeInfo(const StringView& data)
	{
        std::unordered_map<Hashed_String, StringView> dataInfo; // データを入れる配列
        size_t pos = 0; // 今の位置

        while (pos < data.GetSize())
        {
            size_t nextPos = data.GetFind(u8"\n", pos); // １行の終わりの位置
            StringView line; // 一行を入れる

            if (nextPos == std::string::npos)
            {
                line = StringView(data.GetData() + pos, data.GetSize() - pos); // 今の位置から最後までを取り出す
                pos = data.GetSize();       // 位置をデータの最後にする
            }
            else
            {
                line = StringView(data.GetData() + pos, nextPos - pos); // １行を取り出す
                pos = nextPos + 1; // 次の位置を求める
            }

            // 先頭の空白を削除 
            size_t firstNonSpace = line.GetFindFirstNotOf(u8" ");
            if (firstNonSpace != std::string_view::npos)
            {
                line.SetRemovePrefix(firstNonSpace);
            }
            else
            {
                continue; // 空行はスキップ
            }

            // キーと値を分割
            size_t delimPos = line.GetFind(kKey); // 区切り文字が出てくる位置を検索

            if (delimPos != std::string::npos)
            {
                StringView dataType = line.SubStr(0, delimPos); // キーまでを代入
                StringView dataString = line.SubStr(delimPos + kKey.GetSize()); // キーから上を代入
                dataInfo[Hashed_String(dataType)] = dataString; // 配列に代入
            }
        }

        return dataInfo;
	}


    // データからブロックの情報を取り出す
    bool ExtractBlocks(const StringView& data, const String& blockName, StringView& block)
    {
        // ブロック名の位置を探す
        size_t startNamePos = data.GetFind(blockName, 0);
        if (startNamePos == std::string::npos) {
            ErrorLog::OutputToConsole(u8"ブロック名が見つかりませんでした");
            return false;
        }

        // ブロック情報の開始位置を探す
        size_t blockStartPos = data.GetFind(kBlockStart, startNamePos + blockName.GetSize()); // ブロック数の位置を探す
        if (blockStartPos == std::string::npos) {
            ErrorLog::OutputToConsole(u8"ブロック情報の開示位置が見つかりませんでした");
            return false;
        }

        // 対応する終了位置を探す
        size_t blockEndPos;
        if (!FindBlockEnd(data, blockStartPos, blockEndPos)) {
            ErrorLog::OutputToConsole(u8"対応する区切り文字が見つかりませんでした");
            return false;
        }

        // 最初から最後までの範囲を std::string_view に代入
        block = StringView(data.GetData() + startNamePos,
            blockEndPos - startNamePos + kBlockEnd.GetSize());

        return true;
    }


    // ブロックの中身を取得する（指定した blockName の直下ブロックをすべて返す）
    bool ExtractSubBlocks(const StringView& data, const String& blockName, std::vector<StringView>& blocks)
    {
        // ブロック名の位置を探す
        size_t startNamePos = data.GetFind(blockName, 0);
        if (startNamePos == std::string::npos) {
            ErrorLog::OutputToConsole(u8"ブロック名が見つかりませんでした");
            return false;
        }

        // ブロック情報の数を取得する
        size_t BlockNumberPos = data.GetFind(kBlockNumber, startNamePos); // ブロック数の位置を探す
        if (BlockNumberPos == std::string::npos) {
            ErrorLog::OutputToConsole(u8"ブロック情報の数が見つかりませんでした");
            return false;
        }

        // ブロック数の開始位置を求める
        size_t numberStart = BlockNumberPos + kBlockNumber.GetSize();
        // ブロック数の最後位置を求める
        size_t numberEnd = data.GetFindFirstOf(u8"\n\n", numberStart);
        if (numberEnd == std::string::npos) {
            ErrorLog::OutputToConsole(u8"ブロック数の後に改行、空白などがありません");
            return false;
        }

        // 開始と終わりまでの文字列を代入
        String numberStr(data.GetData() + numberStart, numberEnd - numberStart);
        // ブロック数を取得
        int blockNumber = std::stoi(
             reinterpret_cast<const char*>(numberStr.GetU8Char()));

        blocks.resize(blockNumber); // 配列をリサイズする

        // ブロック情報がなければ戻る
        if (blockNumber == 0) {
            return true;
        }

        // ブロック情報の開始位置を探す
        size_t blockInfoStartPos = data.GetFind(kBlockStart, numberEnd); // ブロック数の位置を探す
        if (blockInfoStartPos == std::string::npos) {
            ErrorLog::OutputToConsole(u8"ブロック情報の開示位置が見つかりませんでした");
            return false;
        }

        // ブロック情報を抜き出して、配列に代入させる
        for (int i = 0; i < blockNumber; i++)
        {
            size_t blockInfoEndPos = 0; // ブロック情報の終わり位置を入れる

            // 対応する区切り文字を探す
            if (!FindBlockEnd(data, blockInfoStartPos, blockInfoEndPos)) {
                ErrorLog::OutputToConsole(u8"対応する区切り文字が見つかりませんでした");
                return false;
            }

            // 中身だけを抽出して配列に追加
            StringView blockContent(data.GetData() + blockInfoStartPos + kBlockStart.GetSize(),
                blockInfoEndPos - blockInfoStartPos - kBlockStart.GetSize());
            blocks[i] = blockContent;

            // 次のブロック開始位置に移動
            blockInfoStartPos = data.GetFind(kBlockStart, blockInfoEndPos + kBlockEnd.GetSize());
            if (i + 1 < blockNumber) {
                if (blockInfoStartPos == std::string::npos) {
                    ErrorLog::OutputToConsole(u8"次のブロック開始位置を見つけられませんでした");
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
        bool FindBlockEnd(const StringView& data, const size_t& startPos, size_t& endPos)
        {
            size_t nextStartPos = data.GetFind(kBlockStart, startPos + kBlockStart.GetSize()); // 次の区切り開始位置
            size_t nextEndPos = data.GetFind(kBlockEnd, startPos + kBlockStart.GetSize()); // 次の区切り終了位置

            if (nextEndPos == std::string_view::npos) 
            {
                ErrorLog::OutputToConsole(u8"データに区切り終了文字が含まれていません");
                return false; // 終了文字なし
            }
            int depth = 0; // 深度

            // 対応する区切り文字探し
            while (true)
            {
                if (nextStartPos != std::string_view::npos && nextStartPos < nextEndPos)
                {
                    depth++;
                    nextStartPos = data.GetFind(kBlockStart, nextStartPos + kBlockStart.GetSize());
                }
                else
                {
                    if (depth == 0)
                    {
                        endPos = nextEndPos; // 区切り終了位置を代入
                        return true;
                    }

                    depth--; // 深度を減らす
                    nextEndPos = data.GetFind(kBlockEnd, nextEndPos + kBlockEnd.GetSize()); // 終わりを探す
                    if (nextEndPos == std::string_view::npos) {
                        ErrorLog::OutputToConsole(u8"対応する区切り文字が見つかりませんでした");
                        return false;
                    }
                }
            }

            return false;
        }
}

