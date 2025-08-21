
// =========================================
// ヘッダー
// =========================================
#include "SaveLoadUtils.h" // 必須ヘッダー


// =========================================
// グローバル変数
// =========================================
namespace {
	// データの種類とデータを区切る文字
	const std::string kKey = " :";
}


// =========================================
// セーブ関数
// =========================================
namespace SaveUtils {
	// データの種類とデータ文字列を受け取り、キーを付けて文字列にして返す
	std::string MakeSaveData(std::string dataType, std::string data, int spaceNumber)
	{
		std::string saveData(spaceNumber, ' '); // 空白を開ける

		saveData += dataType + kKey + data; // セーブ文字列を作成

		return saveData;
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


}

