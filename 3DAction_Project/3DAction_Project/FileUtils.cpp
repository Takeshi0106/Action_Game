
// ==================================
// ヘッダー
// ==================================
// 必須ヘッダー
#include "FileUtils.h"
// 文字列
#include <string> // 文字列
#include <string_view> // 文字列参照
// 外部ファイル操作
#include <fstream>
// ファイルパス用ヘッダー
#include <filesystem>
// メッセージ出力
#include "ReportMessage.h"


// ==================================
// 関数
// ==================================
namespace FileUtis 
{
	// 書き出すファイル
	bool WriteFile(const std::string& filePath, const std::string_view& data)
	{
        // フォルダがない場合作成
        if (!std::filesystem::exists(std::filesystem::path(filePath).parent_path())) {
            if (!std::filesystem::create_directories(std::filesystem::path(filePath).parent_path())) {
                ErrorLog::OutputToConsole("ファイルが作成できませんでした");
                return false;
            }
        }

        std::ofstream ofs(filePath, std::ios::binary | std::ios::out); // バイナリモードで書き出し
        if (!ofs)
        {
            ErrorLog::OutputToConsole(("ファイルを開けませんでした: " + filePath).c_str());
            return false;
        }

        ofs.write(data.data(), data.size()); // データを書き込む

        if (!ofs)
        {
            ErrorLog::OutputToConsole(("ファイル書き込みに失敗しました: " + filePath).c_str());
            return false;
        }

        return true;
    }


	// 読み込むファイル
	bool ReadFile(const std::string& path, std::string& data)
	{
        std::ifstream ifs(path, std::ios::binary); // 開く
        if (!ifs) {
            ErrorLog::OutputToConsole(("ファイルを開けませんでした: " + path).c_str());
            return false;
        }

        ifs.seekg(0, std::ios::end); // サイズを取得
        size_t fileSize = static_cast<size_t>(ifs.tellg()); // 先頭から現在の位置の長さを取得
        ifs.seekg(0, std::ios::beg); // 最初に戻す

        data.resize(fileSize); // サイズを決める

        ifs.read(data.data(), fileSize);

        ifs.close(); // 一応閉じる

        return true; // 読み込み成功確認
	}


}

