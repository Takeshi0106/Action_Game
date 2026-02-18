
// ==================================
// ヘッダー
// ==================================
// 必須ヘッダー
#include "FileUtils.h"
// バイナリーデータクラス
#include "BinaryData.h"
// バイナリー参照データ
#include "BinaryView.h"
// 外部ファイル操作
#include <fstream>
// ファイルパス用ヘッダー
#include <filesystem>
// メッセージ出力
#include "ReportMessage.h"
// バッファ用配列
#include <vector>


// ==================================
// 関数
// ==================================
namespace FileUtis 
{
    // =========================================
    // テキストファイル IO
    // =========================================
    namespace Text
    {
        // --------------------------------
        // 書き込み
        // --------------------------------
        bool WriteTextFile(const String& _path, const String& data)
        {
            // ファイルパスを作成
            std::filesystem::path filePath(_path.GetU8String());

            // フォルダがない場合作成
            if (!std::filesystem::exists(filePath.parent_path()))
            {
                if (!std::filesystem::create_directories(filePath.parent_path()))
                {
                    ErrorLog::OutputToConsole(u8"ファイルが作成できませんでした");
                    return false;
                }
            }

            // ファイルオープン
            std::ofstream ofs(std::filesystem::path(_path.GetU8String()), std::ios::binary);

            // エラーチェック
            if (!ofs) {
                ErrorLog::OutputToConsole((u8"ファイルを開けませんでした: " + _path));
                return false;
            }

            // バイナリーView
            BinaryView dataView = data.GetBinaryView();

            // 書き出し
            ofs.write(reinterpret_cast<const char*>(dataView.GetData()), dataView.GetSize());

            // 書き込みチェック
            if (!ofs)
            {
                ErrorLog::OutputToConsole((u8"ファイル書き込みに失敗しました: " + filePath.u8string()));
                return false;
            }

            // 一応閉じる
            ofs.close();

            return true;
        }


        // --------------------------------
        // 読み込むファイル
        // --------------------------------
        bool ReadTextFile(const String& path, String& data)
        {
            // バイナリモードで読み込み
            std::ifstream ifs(std::filesystem::path(path.GetU8String()), std::ios::binary);

            // ファイルが開けたかチェック
            if (!ifs) {
                ErrorLog::OutputToConsole((u8"ファイルを開けませんでした: " + path));
                return false;
            }

            // サイズを取得
            ifs.seekg(0, std::ios::end);
            // 先頭から現在の位置の長さを取得
            size_t fileSize = static_cast<size_t>(ifs.tellg());
            // 最初に戻す
            ifs.seekg(0, std::ios::beg);

            // バッファ
            std::vector<char8_t> buffer(fileSize);
            // データを読み込む
            ifs.read(reinterpret_cast<char*>(buffer.data()), fileSize);
            // 読み込みチェック
            data = String(std::u8string(buffer.begin(), buffer.end()));

            // 一応閉じる
            ifs.close();

            return true;
        }
    }


	// =========================================
    // バイナリーデータ　IO
	// =========================================
    namespace Binary
    {
        // --------------------------------
        // 書き出すファイル
        // --------------------------------
        bool WriteBinaryFile(const String& filePath, const BinaryView& data)
        {
            // フォルダがない場合作成
            if (!std::filesystem::exists(
                std::filesystem::path(filePath.GetU8String()).parent_path()))
            {
                if (!std::filesystem::create_directories(std::filesystem::path(filePath.GetU8String()).parent_path()))
                {
                    ErrorLog::OutputToConsole(u8"ファイルが作成できませんでした");
                    return false;
                }
            }

            // バイナリモードで書き出し
            std::ofstream ofs(
                std::filesystem::path(filePath.GetU8String()),
                std::ios::binary | std::ios::out);

            // ファイルが開けたかチェック
            if (!ofs)
            {
                ErrorLog::OutputToConsole((u8"ファイルを開けませんでした: " + filePath.GetU8String()));
                return false;
            }

            // データを書き込む
            ofs.write(
                reinterpret_cast<const char*>(data.GetData()),
                data.GetSize());

            // 書き込みチェック
            if (!ofs)
            {
                ErrorLog::OutputToConsole((u8"ファイル書き込みに失敗しました: " + filePath.GetU8String()));
                return false;
            }

            // 一応閉じる
            ofs.close();

            return true;
        }


        // --------------------------------
        // 読み込むバイナリーデーター
        // --------------------------------
        bool ReadBinaryFile(const String& _filePath, BinaryData& _data)
        {
            std::filesystem::path filePath(_filePath.GetU8String());

            // バイナリモードで読み込み
            std::ifstream ifs(filePath, std::ios::binary);

            // ファイルが開けたかチェック
            if (!ifs) {
                ErrorLog::OutputToConsole((u8"ファイルを開けませんでした: " + _filePath.GetU8String()));
                return false;
            }
            // サイズを取得
            ifs.seekg(0, std::ios::end);
            size_t fileSize = static_cast<size_t>(ifs.tellg());
            ifs.seekg(0, std::ios::beg);

            // バッファ
            std::vector<std::byte> buffer(fileSize);

            // データを読み込む
            ifs.read(reinterpret_cast<char*>(buffer.data()), fileSize);
            // 読み込みチェック
            if (!ifs)
            {
                ErrorLog::OutputToConsole((u8"ファイルの読み込みに失敗しました: " + _filePath.GetU8String()));
                return false;
            }

            // データをセット
            _data.Write(buffer.data(), buffer.size());

            // 一応閉じる
            ifs.close();

            return true;
        }
    }
}

