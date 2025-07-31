#pragma once

namespace ErrorLog {
	// HRESULTとメッセージボックスで出力するメッセージを引き数で渡す　失敗時に戻り値はtrueを返す
	bool IsSuccessHRESULTWithMessageBox(long hr, const char* message);
	// hrの判定と、失敗時にデバッグ時のみコンソールに出力
	bool IsSuccessHRESULTWithOutputToConsole(long hr, const char* message);
	// メッセージボックスを出力させる
	void MessageBoxOutput(const char* message);
	// コンソールに出力させるエラーログ　リリース時はログ情報を書き出せるよう変更予定
	void Log(const char* message);
}

namespace Debug {
#if defined(DEBUG) || defined(_DEBUG)
	// デバッグ用ログ出力
	void NormalLog(const char* message);
#else
	// リリース時はインラインでビルド時に消えるかもしれない
	inline void NormalLog(const char* message) {}
#endif
}